// STD I/O
#include <iostream>
#include <fstream>

// STD Containers
#include <unordered_map>

// Other Types
#include <ctime>    // Wallclock time to deprecate minibuffer messages
#include <cstring>

// Curses Framework
#include <ncurses.h>
#include <curses.h> // keyname -- Will this help with ctrl modifiers?


#include "buffer.h"
#include "calculator/converter.h"
#include "calculator/evaluate.h"


//#define DEBUG


class editor {
 private:
  int row, col;
  int MAX_ROW, MAX_COL;
  bool INSERT_MODE, EXIT;
  buffer text_buffer;
  WINDOW *main_window,  *minibuffer;
  std::string file_name;
  std::string last_autosave_file;
  time_t last_message_time; // Time the last minibuffer message was posted.
  time_t last_save_time;    // Time since last save (or autosave)
  
  // User accessible variables available to the "Python" language.
  std::unordered_map<std::string, float> global_vars;
  
 public:
  editor();
  void init();
  
  void check_clock();
  
  /*
    Basic keystroke handlers.
   */
  void handle_keypress(int key);
  void handle_arrow(int key);
  void handle_backspace();
  void handle_control_key(int key);
  void handle_esc();
  void handle_return();
  
  void insert_character(int key);
  
  void move_cursor(int row, int col);
  

  /*
    Editor Control
   */
  void main_loop();
  
  /*
    File IO Methods
   */
  void auto_backup();
  void delete_autosave_file();
  void read_file(std::string file);
  void write_file();
  void write_file(std::string file);
  
  /*
    Window Methods
   */
  void update_screen();
  void resize_windows();
  
  /*
    Minibuffer Methods
   */
  void clear_minibuffer();
  void enter_minibuffer();
  void minibuffer_message(std::string message);
  std::string minibuffer_input();
  std::string minibuffer_input(std::string prompt);
  
  
  /*
    Text Processing Methods
   */
  std::string trim(std::string word);
  bool is_arithmetic(std::string word);
  
  
  /*
    Python methods and handlers.
  */
  void handle_assignment(std::string statement);
  bool identifier_exists(std::string identifier);
  void handle_print(std::string message);
  float process_arithmetic(std::string expression);
  /*
   * Note: Calls postfix_evaluate.value() which may throw an exception if:
   *       -identifier doesn't exist or
   *       -divide by zero
   */
};

