
#include "editor.h"

using namespace std;

const int CARRIAGE_RETURN = 13;
const int NEWLINE = 10;
const int ALT_BACKSPACE = 127;
const int KEY_ESC=27;

extern void perror(), exit();


editor::editor() {
  init();
}

void editor::init() {
  //Initialize Object
  row = col = 0;
  INSERT_MODE = true;
  EXIT = false;
  file_name = "";
  last_autosave_file = "";
  last_message_time = 0;
  last_save_time = time(NULL);
  
  global_vars["message_delay"] = 3;
  global_vars["autosave_delay"] = 30;
  
  //Reduce Default ncurses Escape Delay
  if (getenv ("ESCDELAY") == NULL)
    ESCDELAY = 25;

  
  //Initialize ncurses state
  
  //Initialize screen
  initscr();

  getmaxyx(stdscr, MAX_ROW, MAX_COL);
  
  //Read keystrokes immediately
  cbreak();
  //Leave newline mode.
  nonl();
  //Turn off character echoing
  noecho();
  //Attempt to use hardware insert/delete line feature of the terminal
  idlok(stdscr, TRUE);
  keypad(stdscr, TRUE);
  
  //Do not block on getch() input
  nodelay(stdscr, TRUE);
  
  
  refresh();
  main_window = newwin(MAX_ROW-2, 0, 1, 0);
  minibuffer = newwin(0, 0, MAX_ROW-1, 1);

  box(main_window, 0, 0);
  //box(minibuffer, 0, 0);
  move_cursor(row, col);
  wmove(stdscr, 0, 0);
  wprintw(stdscr, " Edit");
  wrefresh(stdscr);
  minibuffer_message("Welcome to Edit!");
  wrefresh(main_window);
  wrefresh(minibuffer);
  
  //update_screen();
  //box(main_window, 0, 0);
  //wrefresh(main_window);
}


void editor::check_clock() {
  time_t current_time = time(NULL);
  
  // Is there message in the minibuffer?
  if(last_message_time != 0) {
    // Check whether minibuffer reset time has elapsed.
    if(difftime(current_time, last_message_time) >= global_vars["message_delay"]) {
      clear_minibuffer();
      //Remove countdown
      last_message_time = 0;
      
      
      //Don't leave cursor in minibuffer
      update_screen();
      //refresh();
    }
  }
  
  // Check whether it is time for an autosave.
  if(difftime(current_time, last_save_time) >= global_vars["autosave_delay"]) {
    last_save_time = time(NULL);
    //minibuffer_message("Time for an autosave.");
    
    // Initiate Autosave
    auto_backup();
    // if(file_name == "") {
    //   minibuffer_message("No file to save to.");
    //   write_file("#edit_buffer#");
    // }
    // else {
    //   minibuffer_message("Autosaving to file: " + file_name);
    // }
  }
  
  
}


void editor::handle_keypress(int key) {
  
  //printw("row %d col %d", row, col);
  //cout << "test" << endl;
  switch(key) {
  case KEY_UP:
  case KEY_DOWN:
  case KEY_LEFT:
  case KEY_RIGHT:
    handle_arrow(key);
    break;
    
  case KEY_ENTER:
  case NEWLINE:
  case CARRIAGE_RETURN:
    handle_return();
    break;

  case KEY_BACKSPACE:
  case ALT_BACKSPACE:
    handle_backspace();
    break;
    
  case KEY_ESC:
    handle_esc();
    break;
    
  case KEY_F(1):
    INSERT_MODE = !INSERT_MODE;
    break;
    
  case 32 ... 126:
    insert_character(key);
    break;

  // default:
  //   clear();
  //   printw("Key = %d\n", key);
  }
  
}


void editor::handle_arrow(int key) {
  switch(key) {
  case KEY_UP:
    //Bounds check top wall
    if(row != 0) {
      row--;
      
      //Shorter row has been entered.
      if(text_buffer[row].size() < col)
	// Move to end of row.
	col = text_buffer[row].size();
    }
    //Can't move, ring bell.
    else
      cout << '\a';
    
    move_cursor(row, col);
    break;

  case KEY_DOWN:
    //Bounds check bottom of buffer
    //Row exists, buffer not empty
    if(row < text_buffer.size()-1 && text_buffer.size() != 0) {
      row++;
      
      // Shorter row has been entered.  
      if(text_buffer[row].size() < col)
	// Move to end of row.
	col = text_buffer[row].size();
    }
    //Can't move, ring bell.
    else
      cout << '\a';
    
    move_cursor(row, col);
    break;

  case KEY_LEFT:
    //Bounds check left wall
    if(col != 0)
      col--;
    //Left side line wrap
    else if(col == 0 && row > 0) {
      row--;
      col = text_buffer[row].size();
    }
    //Can't move, ring bell.
    else
      cout << '\a';
    
    move_cursor(row, col);
    break;

  case KEY_RIGHT:
    //Bounds check right buffer (short circuit empty buffer)
    if(text_buffer.size() != 0 && col < text_buffer[row].size())
      col++;
    //End of row wrap
    else if(row < text_buffer.size()-1 && text_buffer.size() != 0) {
      row++;
      col=0;
    }
    //Can't move (eof), ring bell
    else
      cout << '\a';
    
    move_cursor(row, col);
    //wrefresh(stdscr);
    break;
  }
}


void editor::handle_backspace() {
  //Backspace at the start of a line
  if(col==0 && row > 0) {
    //Find new cursor position
    int next_col = text_buffer[row-1].size();

    //Append line to previous line
    text_buffer[row-1] += text_buffer[row];
    //Remove current line
    text_buffer.erase(row);
    
    row--;
    col = next_col;
  }
  //Backspace inside/at the end of a line
  else if(col != 0) {
    col--;
    text_buffer[row].erase(col, 1);
  }
  else
    cout << '\a';
}

// Minibuffer input requested.
// Handle common inputs
void editor::handle_esc() {
  string message = minibuffer_input();
  
  // Exit program
  if(message == "quit()")
    EXIT = true;
  // Output buffer to file.
  else if(message == "write()")
    write_file();
  // Write to supplied file (in quotes)
  else if(message.find("write(\"") == 0 &&
	  message[message.size()-1] == ')' && 
	  message[message.size()-2] == '"') {
    //Extract file name and write.
    message = message.substr(7, message.size()-9);
    minibuffer_message("Writing buffer to file: " + message);
    write_file(message);
  }
  // Print method called.
  else if(message.find("print(") == 0 &&
	  message[message.size()-1] == ')') {
    //Extract file name and write.
    message = message.substr(6, message.size()-7);
    
    //Handle message to be printed
    handle_print(message);
  }
  // Assignment Statement
  else if(message.find('=') != string::npos) {
    handle_assignment(message);
  }
  // Identifier in system
  else if(identifier_exists(message)) {
    minibuffer_message(to_string(global_vars[message]));
  }
  else if(!identifier_exists(message) && message != "") {
    minibuffer_message("NameError: name '" + message + "' is not defined");
  }
}


void editor::handle_return(){
  string new_line="";
  
  //Empty buffer
  if(text_buffer.size() == 0) {
    text_buffer.push_back(new_line);
    text_buffer.push_back(new_line);
    handle_arrow(KEY_DOWN);
  }
  //Return at the end of a row
  else if(col == text_buffer[row].size()) {
    //Insert a new line of text
    text_buffer.insert(row+1, new_line);
    
    //Move cursor and error check
    handle_arrow(KEY_DOWN);
  }
  //Return at start of a row
  else if(col == 0) {
    //Insert a new line of text
    text_buffer.insert(row, new_line);
    
    //Move cursor and error check
    handle_arrow(KEY_DOWN);
  }
  //Newline in current line
  else {
    //Split end of line
    new_line = text_buffer[row].substr(col);
    
    //Truncate current line
    text_buffer[row] = text_buffer[row].substr(0, col);
    
    //Insert split line
    text_buffer.insert(row+1, new_line);
    
    //Move cursor and error check
    handle_arrow(KEY_DOWN);
    col = 0;
    move_cursor(row, col);
  }
  
  update_screen();
}

void editor::insert_character(int key) {
  string temp;
  
  //No room in buffer
  if(text_buffer.size() == 0)
    text_buffer.push_back(temp);
  
  //End of the current row
  if(text_buffer[row].size() == col){
    text_buffer[row].append(1, key);
    col++;
  }
   
  //Room in buffer at desired location
  else if(INSERT_MODE) {
    //Insert typed text 
   text_buffer[row].insert(col, 1, char(key));
    col++;
  }
  else {
    //Overwrite typed text
    text_buffer[row][col] = char(key);
    col++;
  }
  
  // string message = "Wrote character: ";
  // message += char(key);
  // minibuffer_message(message);
}


void editor::move_cursor(int row, int col) {
  // row+1, col+1 to account for border.
  wmove(main_window, row+1, col+1);
}



void editor::main_loop() {
  int key=0;
  
  //cout << "in main loop" << endl;
  // cin >>key;
  
  //Initialize empty buffer
  if(text_buffer.size() == 0)
    text_buffer.push_back("");
  
  do {
    resize_windows();
    check_clock();
    
    if(key != ERR)
      update_screen();
    //    enter_minibuffer();
    key = getch();
    
    if(key != ERR)
      handle_keypress(key);
    
  } while (!EXIT); 
  
  clear();
  refresh();
  endwin();
  return;
  
}



/*
  File IO Methods
 */


void editor::auto_backup() {
  string backup_name;
  if(file_name == "") {
    backup_name = "#edit_backup#";
  }
  else if(file_name.find('/') == string::npos) {
    backup_name = "#" + file_name + "#";
  }
  else {
    int path_position = file_name.find_last_of('/');
    backup_name = file_name.substr(0, path_position) + "/#" + file_name.substr(path_position+1) + "#";
  }
  
  minibuffer_message("Autosaving backup file");
  
  ofstream backup(backup_name.c_str());
  
  if(!backup) {
    minibuffer_message("Failed to open auto backup file: " + backup_name);
    return;
  }
  // else
  //   minibuffer_message("Writing output to file: " + backup_name);
  
  //Write data if buffer is nonempty
  if( !(text_buffer.size() == 1 && text_buffer[0].size() == 0) )
    for(int i=0; i<text_buffer.size(); i++) {
      backup << text_buffer[i] << endl;
    }
  
  backup.close();
  
  minibuffer_message("Autosaving backup file...done.");
  
  last_autosave_file = backup_name;
}

void editor::delete_autosave_file() {
  if(last_autosave_file != "")
    remove(last_autosave_file.c_str());
  last_autosave_file = "";
}

void editor::read_file(string file) {
  string current="";
  file_name = file;
  
  ifstream infile(file_name);
  
  getline(infile, current);
  
  while(infile) {
    text_buffer.push_back(current);
    
    getline(infile, current);
  }
  infile.close();
}


void editor::write_file() {
  if(file_name == "") {
    file_name = minibuffer_input("Please enter a file name: ");
    minibuffer_message("You entered: " + file_name);
  }
  
  ofstream outfile(file_name.c_str()), backup((file_name + "~").c_str());
  
  if(!outfile) {
    minibuffer_message("Failed to open output file: " + file_name);
    file_name = "";
    return;
  }
  else
    minibuffer_message("Writing output to file: " + file_name);
  
  for(int i=0; i<text_buffer.size(); i++) {
    outfile << text_buffer[i] << endl;
    backup  << text_buffer[i] << endl;
  }
  
  outfile.close();
  backup.close();
  
  delete_autosave_file();
}


void editor::write_file(string file) {
  file_name = file;
  write_file();
}


/*
  Window Methods
 */

void editor::update_screen() {
  wclear(main_window);

  // printw("Edit File");
  // row++;
  
  if(text_buffer.size() == 0) {
    #ifdef DEBUG
    wprintw(main_window, "##");
    #endif
    //    box(main_window, 0, 0);
    move_cursor(row, col);
    wrefresh(main_window);
    return;
  }
  
  box(main_window, 0, 0);
  
  //Print current buffer to the screen.
  for(int i=0; i<text_buffer.size(); i++) {
    move_cursor(i, 0);
    wprintw(main_window, text_buffer[i].c_str());
    
    #ifdef DEBUG
    //Mark end of line
    move_cursor(i, text_buffer[i].size());
    wprintw(main_window, "#");
    #endif
  }
  
  #ifdef DEBUG
  //Mark end of file
  int last_row = text_buffer.size()-1;
  move_cursor(last_row, text_buffer[last_row].size());
  wprintw(main_window, "##");
  #endif
  
  move_cursor(row, col);
  wrefresh(main_window);
}


void editor::resize_windows() {
  getmaxyx(stdscr, MAX_ROW, MAX_COL);
  main_window = newwin(MAX_ROW-2, 0, 1, 0);
  minibuffer = newwin(0, 0, MAX_ROW-1, 1);
}



/*
  Minibuffer Methods
 */


void editor::clear_minibuffer() {
  wclear(minibuffer);
  wrefresh(minibuffer);
}


void editor::enter_minibuffer() {
  // window_focus = minibuffer;
  // wmove(minibuffer, 0, 0);
  wmove(stdscr, MAX_ROW, 0);
}

void editor::minibuffer_message(string message) {
  wclear(minibuffer);
  wmove(minibuffer, MAX_ROW-1, 1);
  wprintw(minibuffer, message.c_str());
  wrefresh(minibuffer);
  wmove(main_window, row, col);
  
  last_message_time = time(NULL);
}

string editor::minibuffer_input() {
  return minibuffer_input(">>> ");
}


string editor::minibuffer_input(string prompt) {
  int key;
  string return_val;
  
  wclear(minibuffer);
  wmove(minibuffer, MAX_ROW-1, 1);
  wprintw(minibuffer, prompt.c_str());
  wrefresh(minibuffer);
  
  key = getch();
  while(key != KEY_ENTER && key != NEWLINE && key != CARRIAGE_RETURN) {
    if(key == KEY_BACKSPACE || key == ALT_BACKSPACE) {
      return_val.pop_back();
      minibuffer_message(prompt + return_val);
    }
    else if(key == KEY_ESC) {
      //Quit minibuffer
      return_val = "";
      clear_minibuffer();
      move_cursor(row, col);
      return return_val;
    }
    else
      if(key != ERR) {
	return_val += char(key);
	
	// Probably inefficient.  Should be processing
	// one character at a time rather than
	// clearing and printing the entire message.
	minibuffer_message(prompt + return_val);
      }
    
    key = getch();
  }
  
  return return_val;
}


/*
  Text Processing Methods
*/
string editor::trim(string word) {
  size_t first = word.find_first_not_of(' ');
  size_t last = word.find_last_not_of(' ');
  return word.substr(first, (last-first+1));

}


bool editor::is_arithmetic(std::string word) {
  if(word.find('+')  != string::npos ||
     word.find('-')  != string::npos ||
     word.find('/')  != string::npos ||
     word.find('*')  != string::npos ||
     word.find("**") != string::npos) {
    
    //minibuffer_message("Arithmetic operator found");

    return true;
  }
  return false;
}



/*
  Python lookup methods.
*/

void editor::handle_assignment(string statement) {
    // Stub, come back and do later.
    string lhs, rhs;
    int rhs_length=0;
    float rhs_value;
    
    rhs_length = statement.length() - (statement.find('=')+1);
    
    //Extract lhs, rhs of assignment
    lhs = statement.substr(0, statement.find('='));
    rhs = statement.substr(statement.find('=')+1, rhs_length);
    
    //Trim Strings
    lhs = trim(lhs);
    rhs = trim(rhs);
    
    //Check to see if RHS is a variable or literal
    rhs_value = atof(rhs.c_str());
    
    // lhs variable contains spaces
    if(lhs.find(' ') != string::npos)
      minibuffer_message("Invalid Syntax: " + statement.substr(0, statement.find('=')+1));
    // rhs is arithmetic
    else if(is_arithmetic(rhs)) {
      
      try {
	double value = process_arithmetic(rhs);
	global_vars[lhs] = value;
      }
      catch (exception &e) {
	// Most likely variable doesn't exist or
	// Divide by zero
	minibuffer_message(e.what());
      }
      
    }
    // lhs variable valid, rhs is float or int
    else if(rhs_value != 0)
      global_vars[lhs] = rhs_value;
    // lhs variable valid, rhs is a variable that doesn't exist
    else if( !identifier_exists(rhs) )
      minibuffer_message("NameError: name '" + rhs + "' is not defined");
    // lhs variable valid, rhs variable valid
    else
      global_vars[lhs] = global_vars[rhs];
}


bool editor::identifier_exists(string identifier) {
  if(global_vars.find(identifier) == global_vars.end())
    return false;
  else
    return true;
}



void editor::handle_print(string message) {
  //Attempt to process numeric messages
  float float_msg = atof(message.c_str());
  int int_msg = atoi(message.c_str());
    
  // Printing a literal string
  if(message[0] == '"' && message[message.size()-1] == '"')
    minibuffer_message(message.substr(1, message.size()-2));
  // Printing an integer value
  else if(int_msg != 0 && (float_msg-int_msg)==0)
    minibuffer_message("Integer point value: " + to_string(int_msg));
  // Printing a floating point value
  else if(float_msg != 0)
    minibuffer_message("Floating point value: " + to_string(float_msg));
  else if(identifier_exists(message))
    minibuffer_message("Look up value: " + to_string(global_vars[message]));
}


float editor::process_arithmetic(std::string expression) {
  postfix_converter c(expression);
  string postfix_expression = c.postfix();
  postfix_evaluate e(postfix_expression);
  double return_value;
  
  return_value = e.value(global_vars);
  
  return return_value;
}
