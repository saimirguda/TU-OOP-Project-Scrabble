//-----------------------------------------------------------------------------
// a3.c
//
// This is a program of a Scrabble game. Firstly a configuration file is required
// to be read. Program then saves the file in heap memory and the game begins.
// Program prints the board and awaits for a user command. Each command performs
// a different task(insert word into board, save or load the game and print help)
//
// Group: Michael H 07
//
// Author: 11933108
//-----------------------------------------------------------------------------
//

#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define TOKEN_SEPARATORS " \t\n"
#define BONUS

typedef struct _Config_{
  char *file_;
  char *magic_word_;
  char *board_;
  char* player_turn_;
  char* player_1_score_;
  char* player_2_score_;
  char *letter_score_;
  char *original_letter_score_;
  int victory_score_;
} Config;

typedef enum _Command_ {
  INSERT,
  SAVE,
  QUIT,
  HELP,
  UNKNOWN
#ifdef BONUS
  ,LOAD
#endif

} Command;

typedef enum _Orientation_ {
  HORIZONTAL,
  VERTICAL
} Orientation;

typedef struct _Input_ {
  Command command_;
  bool is_error_;
  char row_;
  char column_;
  Orientation orientation_;
  char *word_;
  bool only_whitespaces_;
#ifdef BONUS
  char* load_config_;
#endif
} Input;

void stringToLower(char *string);

bool parseOrientationParameter(Input *input, char *token);

void parseInsertCommand(Input *input, int num_lines);

void parseLoadCommand(Input *input);

int num_of_lines(char *the_file);


//-----------------------------------------------------------------------------
///
/// This function checks if a letter is lower case
///
/// @param chosen_letter the letter to check
///
/// @return true if the letter is within the determined ranges and flase if not
//
bool lower(char chosen_letter)
{
  return chosen_letter >= 'a' && chosen_letter <= 'z';
}

//-----------------------------------------------------------------------------
///
/// This function checks if a letter is upper case
///
/// @param chosen_letter the letter to check
///
/// @return true if the letter is within the determined ranges and flase if not
//
bool upper(char chosen_letter)
{
  return chosen_letter >= 'A' && chosen_letter <= 'Z';
}

//-----------------------------------------------------------------------------
///
/// Function parses the user input and checks if it matches correctly
///
/// @param line user input provided by the user(input taken by another function)
/// @param input This is one of our structs. Based on the user input one of the 
/// commands is applied on the struct matching the enumerate Commands.
/// @param config This parameter is used to update the board based on the input
///
/// @return this function has no return value
//
void parseCommand(char *line, Input *input, Config *config)
{
  memset(input, 0, sizeof(Input));

  char* token = strtok(line, TOKEN_SEPARATORS);
  int board_lines = num_of_lines(config->board_);
  stringToLower(token);
  input->only_whitespaces_ = false;
  if (token == NULL)
  {
    input->command_ = UNKNOWN;
    input->only_whitespaces_ = true;
  }
  else if (strcmp("insert", token) == 0)
  {
    parseInsertCommand(input, board_lines);
  }
  else if (strcmp("help", token) == 0)
  {
    input->command_ = HELP;
  }
  else if (strcmp("quit", token) == 0)
  {
    input->command_ = QUIT;
  }
  else if (strcmp("save", token) == 0)
  {
    input->command_ = SAVE;
  }
#ifdef BONUS
  else if (strcmp("load", token) == 0)
  {
    parseLoadCommand(input);
  }
#endif
  else
  {
    input->command_ = UNKNOWN;
    input->is_error_ = true;
  }
}

//-----------------------------------------------------------------------------
///
/// Function converts the letters on the string to lowercase.
///
/// @param string The string to be converted
///
/// @return this function has no return value
//
void stringToLower(char *string)
{
  for (size_t i = 0; string != NULL && string[i] != '\0'; ++i)
    {
      string[i] = tolower(string[i]);
    }
}

//-----------------------------------------------------------------------------
///
/// Function parses the user input in case the command chosen is INSERT and updates
/// values in the input struct.
///
/// @param input This is one of our structs. Based on the user input one of the
/// commands is applied on the struct matching the enumerate Commands. Updates other
/// members of the struct
/// @param num_lines This parameter is used to check for errors
///
/// @return this function has no return value
//
void parseInsertCommand(Input *input, int num_lines) {
  input->command_ = INSERT;

  char *token = strtok(NULL, TOKEN_SEPARATORS);
  stringToLower(token);

  if (token == NULL || strlen(token) > 1 || !lower(*token) || (token[0] - 'a') >= num_lines)
  {
    input->is_error_ = true;
    return;
  }

  input->row_ = token[0];

  token = strtok(NULL, TOKEN_SEPARATORS);
  stringToLower(token);

  if (token == NULL || strlen(token) > 1 || !lower(*token) || (token[0] - 'a') >= num_lines)
  {
    input->is_error_ = true;
    return;
  }

  input->column_ = token[0];

  token = strtok(NULL, TOKEN_SEPARATORS);
  stringToLower(token);

  if (!parseOrientationParameter(input, token))
  {
    input->is_error_ = true;
    return;
  }

  token = strtok(NULL, TOKEN_SEPARATORS);
  stringToLower(token);
  if (token == NULL)
  {
    input->is_error_ = true;
    return;
  }

  char *word = malloc(strlen(token) + 1);
  if (word == NULL)
  {
    input->is_error_ = true;
    return;
  }

  strcpy(word, token);
  input->word_ = word;
}

#ifdef BONUS
//-----------------------------------------------------------------------------
///
/// Function parses the user input in case the command chose is LOAD. Stores inside
/// the struct the name of the file to be loaded.
///
/// @param input This is one of our structs. Based on the user input one of the 
/// values may be changed.
///
/// @return this function has no return value
//
void parseLoadCommand(Input *input)
{
  input->command_ = LOAD;

  char *token = strtok(NULL, TOKEN_SEPARATORS);

  if (token == NULL)
  {
    input->is_error_ = true;
    return;
  }
  char* config_string = (char*) malloc(strlen(token) + 1);
  strcpy(config_string, token);
  input->load_config_ = config_string;
}
#endif

//-----------------------------------------------------------------------------
///
/// Function parses the user input and checks if it matches correctly. Function
/// activates onlz if the command chosen is insert. Updates members if input
///
/// @param input This is one of our structs. Based on the user input orientation
/// is updated.
/// @param token This parameter contains the value of orientation
///
/// @return this function has return value true or false in case it goes off succesfuly
/// or not.
//
bool parseOrientationParameter(Input *input, char *token)
{
  if (token == NULL || strlen(token) > 1)
  {
    input->is_error_ = true;
    return false;
  }

  switch (token[0])
  {
  case 'h':
    input->orientation_ = HORIZONTAL;
    break;
  case 'v':
    input->orientation_ = VERTICAL;
    break;
  default:
    return false;
  }
  
  return true;
}

//-----------------------------------------------------------------------------
///
/// Function reads the user input. First it creates place in heap to store the input to.
/// then returns the created string array.
///
///
/// @return this function has return value an array containing the user input
//
char* read_user_input()
{
  int ch, len;
  char* buffer = malloc(1);
  for (len = 0;(ch = getchar()) != '\n' && ch != EOF; len++)
  {
    buffer[len] = ch;
    buffer = realloc(buffer, len + 2);
  }
  if(ch == -1)
  {
    buffer = realloc(buffer, len + 5);
    buffer[0] = 'q';
    buffer[1] = 'u';
    buffer[2] = 'i';
    buffer[3] = 't';
    return buffer;
  }
  if(buffer == NULL)
  {
    return NULL;
  }
  buffer[len] = '\0';
  return buffer;
}

//-----------------------------------------------------------------------------
///
/// Function simply prints sentances aiming to give information to the user. This
/// function activates only when the chosen command is HELP
///
///
/// @return this function has no return value
//
void print_help()
{
  printf("Commands:\n - insert <ROW> <COLUMN> <H/V> <WORD>\n    <H/V> stands for H: horizontal, V: vertical.\n\n");
  printf(" - help\n    Prints this help text.\n\n");
  printf(" - quit\n    Terminates the game.\n\n");
  printf(" - save\n    Saves the game to the current config file.\n\n");
  printf(" - load <CONFIGFILE>\n    load config file and start game.\n");
}

//-----------------------------------------------------------------------------
///
/// Function reads number of lines that are in a string. Lines that end with '\n'
///
///
/// @return this function has return value an integer being the number of lines
//
int num_of_lines(char *the_file)
{
  int i, j = 0;
  for(i = 0; i <= strlen(the_file); i++)
  {
    if(the_file[i] == '\n')
    {
      j++;
    }
  }
  return j;
}

//-----------------------------------------------------------------------------
///
/// Function reads the file previously open on main creates memory on heap to store
/// it and then saves it on the Config struct.
///
/// @param ptr The file which is being read
/// @param config the place where the content of the file is then stored
///
/// @return this function has no return value
//
void read_file(FILE *file, Config *config)
{
  int ch, len;
  char* buffer = malloc(1);
  for (len = 0;(ch = getc(file)) != EOF && ch != EOF; len++)
  {
    buffer[len] = ch;
    buffer = realloc(buffer, len + 2);
  }
  buffer[len] = '\0';
  char *word = malloc(strlen(buffer) + 1);
  strcpy(word, buffer);
  config->file_ = word;

  free(buffer);
}

//-----------------------------------------------------------------------------
///
/// Function reads the magic word in the file. The magic word is then stored separately
/// in heap
///
/// @param config This is the only parameter being used since the word will be stored in
/// in the struct and also the location of the magic word is in the struct(from read_file)
///
/// @return this function has no return value
//
void read_magic_word(Config *config)
{
  int i;
  char* buffer = malloc(1);
  if(config->file_ == NULL)
  {
    config->magic_word_ = NULL;
    return;
  }
  for(i = 0; config[0].file_[i] != '\n'; i++)
  {
    buffer[i] = config[0].file_[i];
    buffer = realloc(buffer, i + 2);
  }
  buffer[i] = '\n';
  config->magic_word_ = buffer;
}

//-----------------------------------------------------------------------------
///
/// Function reads the board in the file. board is then stored separately in heap
///
/// @param config This is the only parameter being used since board will be stored in
/// in the struct and also the location of the board is in the struct(from read_file)
///
/// @return this function has no return value
//
void read_board(Config *config)
{
  int len;
  int i = 0;
  char* buffer = malloc(1);
  for (len = strlen(config->magic_word_); !(config[0].file_[len] == '1' || config[0].file_[len] == '2'); len++)
  {
    buffer[i] = config[0].file_[len];
    buffer = realloc(buffer, len + 2);
    i++;
  }
  config->board_ = buffer;
}

//-----------------------------------------------------------------------------
///
/// Function reads which player has their turn to play in the file. This is then stored 
/// separately in heap
///
/// @param config This is the only parameter being used since starting player will be 
/// stored in the struct and also the location of it is in the struct(from read_file)
///
/// @return this function has no return value
//
void read_player_start(Config *config)
{
  int i;
  int j = strlen(config->board_) + strlen(config->magic_word_);
  if(strlen(config->board_) % 2 != 0)
    j--;
  char* buffer = malloc(1);
  for(i = 0; config[0].file_[j] != '\n'; i++)
  {
    buffer[i] = config[0].file_[j];
    buffer = realloc(buffer, i + 2);
    j++;
  }
  buffer[i] = '\n';
  buffer = realloc(buffer, i + 2);
  config->player_turn_ = buffer;
}

//-----------------------------------------------------------------------------
///
/// Function reads the score of the players in the file. The score is then stored separately
/// in heap
///
/// @param config This is the only parameter being used since the score will be stored in
/// in the struct and also the location of the score is in the struct(from read_file)
///
/// @return this function has no return value
//
void read_player_score(Config *config)
{
  int i;
  int j = strlen(config->board_) + strlen(config->magic_word_) + strlen(config->player_turn_);
  if(strlen(config->board_) % 2 != 0)
    j--;
  char* buffer = malloc(1);
  for(i = 0; config[0].file_[j] != '\n'; i++)
  {
    buffer[i] = config[0].file_[j];
    buffer = realloc(buffer, i + 2);
    j++;
  }
  buffer[i] = '\n';
  buffer = realloc(buffer, i + 2);
  config->player_1_score_ = buffer;

  j = strlen(config->board_) + strlen(config->magic_word_) + strlen(config->player_turn_) + strlen(config->player_1_score_);
  if(strlen(config->board_) % 2 != 0)
    j--;
  char* buffer_00 = malloc(1);
  for(i = 0; config[0].file_[j] != '\n'; i++)
  {
    buffer_00[i] = config[0].file_[j];
    buffer_00 = realloc(buffer_00, i + 2);
    j++;
  }
  buffer_00[i] = '\n';
  buffer_00 = realloc(buffer_00, i + 2);
  config->player_2_score_ = buffer_00;
}

//-----------------------------------------------------------------------------
///
/// Function reads the assigned score of each letter in the file. This is then 
/// stored separately in heap
///
/// @param config This is the only parameter being used since letter scores will be stored in
/// in the struct and also the location of the scores is in the struct(from read_file)
///
/// @return this function has no return value
//
void read_letter_score(Config *config)
{
  int len;
  int j;
  int i = 0;
  j = strlen(config->board_) + strlen(config->magic_word_) + strlen(config->player_1_score_) + strlen(config->player_2_score_) + strlen(config->player_turn_);
  char* buffer = malloc(1);
  if(config[0].file_[j] == '\n')
  {
    j++;
  }
  for (len = j; config[0].file_[len] != '\n'; len++)
  {
    if(config[0].file_[len] == ' ')
    {
      buffer[i] = ',';
      buffer = realloc(buffer, len + 2);
      i++;
    }
    buffer[i] = config[0].file_[len];
    if(strlen(buffer) == 36 || strlen(buffer) == 73)
    {
      i++;
      buffer[i] = '\n';
      buffer = realloc(buffer, len + 2);
    }
    if(lower(buffer[i]))
    {
      buffer[i] = toupper(buffer[i]);
    }
    buffer = realloc(buffer, len + 2);
    i++;
  }
  j = strlen(config->board_) + strlen(config->magic_word_) + strlen(config->player_1_score_) +
  strlen(config->player_2_score_) + strlen(config->player_turn_);
  if(config[0].file_[j] == '\n')
  {
    j++;
  }
  char* buffer_00 = malloc(1);
  for(i = 0; config[0].file_[j] != '\n'; i++)
  {
    buffer_00[i] = config[0].file_[j];
    buffer_00 = realloc(buffer_00, i + 2);
    j++;
  }
  buffer_00[i] = '\n';
  buffer_00 = realloc(buffer_00, i + 2);
  config->original_letter_score_ = buffer_00;
  config->letter_score_ = buffer;
}

//-----------------------------------------------------------------------------
///
/// Function prints the board in which the players play
///
/// @param config This is the only parameter being used since every info which needs
/// to be printed exists in the Config struct(from previous read functions)
///
/// @return this function has no return value
//
void print_board(Config *config)
{
  int i;
  int board_lines;
  char alpha[27];
  board_lines = num_of_lines(config->board_);
  int  board_size = strlen(config->board_);
  if(board_size % 2 != 0)
    board_size--;
  for(i = 0; i < 26; i++)
  {
    alpha[i] = 'A' + i;
  }
  printf("\n%s\n", config->letter_score_);
  if(strlen(config->player_1_score_) == 3)
  {
    printf("  P1:   %d Points\n", atoi(config->player_1_score_));
  }
  else if(strlen(config->player_1_score_) == 4)
  {
    printf("  P1:  %d Points\n", atoi(config->player_1_score_));
  }
  else
    printf("  P1:    %d Points\n", atoi(config->player_1_score_));
  if(strlen(config->player_2_score_) == 3)
  {
    printf("  P2:   %d Points\n", atoi(config->player_2_score_));
  }
    else if(strlen(config->player_2_score_) == 4)
  {
    printf("  P2:  %d Points\n", atoi(config->player_2_score_));
  }
  else
    printf("  P2:    %d Points\n", atoi(config->player_2_score_));
  printf(" |%*.*s\n", board_lines,board_lines, alpha);
  for(i = 0; i < board_lines + 2; i++)
  {
    printf("-");
  }
  int j = 1;
  printf("\n");
  for(i = 0; i < board_size - 1; i++)
  {
    if(i == 0)
    {
      printf("%c|", alpha[0]);
    }
    printf("%c", config->board_[i]);
    if(config->board_[i] == '\n')
    {
      printf("%c|", alpha[j]);
      j++;
    }
  }
  printf("\n\n");
  printf("Player %d > ", atoi(config->player_turn_));
}

//-----------------------------------------------------------------------------
///
/// Function checks whether the input of the user is correct or not. The word being
/// inserted must conect with an existing word in the board.
///
/// @param input The word which the user inputed exists in the input struct. Other
/// parameters are used to find position of the word
/// @param config The existing information in the board is compared with what 
/// the user inputed
///
/// @return this function has no return value
//
void check_move(Input *input, Config *config)
{
  int i, j = 1;
  int word_num;
  j = num_of_lines(config->board_);
  char a = input->row_;
  char b = input->column_;
  i = j * (a - 'a') + b - 'a' + a - 'a';
  int c = input->orientation_;
  input->is_error_ = true;
  if(config->player_1_score_[0] == '0' && config->player_2_score_[0] == '0')
  {
    input->is_error_ = false;
    return;
  }
  for(word_num = 0; word_num < strlen(input->word_); word_num++)
  {
    if(c == 0)
    {
      if(config->board_[i] == toupper(input->word_[word_num]))
      {
        input->is_error_ = false;
      }
      else if(config->board_[i] != ' ' && config->board_[i] != toupper(input->word_[word_num]))
      {
        input->is_error_ = true;
        return;
      }
      i++;
    }
    else
    {
      if(config->board_[i] == toupper(input->word_[word_num]))
      {
        input->is_error_ = false;
      }
      else if(config->board_[i] != ' ' && config->board_[i] != toupper(input->word_[word_num]))
      {
        input->is_error_ = true;
        return;
      }
      i = i + j;
      i++;
    }
  }
}

//-----------------------------------------------------------------------------
///
/// Function checks whether the input of the user is correct or not. The word being
/// inserted must only contain letters.
///
/// @param input The word which the user inputed exists in the input struct. Other
/// parameters are used to find position of the word
///
/// @return this function has no return value
//
void check_move_2(Input *input)
{
  int i;
  for(i = 0; i < strlen(input->word_); i++)
  {
    if(!(upper(toupper(input->word_[i]))))
    {
      input->is_error_= true;
    }
  }
}

//-----------------------------------------------------------------------------
///
/// Function checks whether the input of the user is correct or not. The word being
/// inserted must contain only letters from the letters list provided.
///
/// @param input The word which the user inputed exists in the input struct.
/// @param config The existing information in the letter_score is compared with what 
/// the user inputed
///
/// @return this function has no return value
//
void check_move_3(Input *input, Config *config)
{
  int i;
  int j = 0;
  int count = 0;
  for(i = 0; i < strlen(config->letter_score_); i++)
  {
    if(toupper(input->word_[j]) == toupper(config->letter_score_[i]))
    {
      j++;
      i = -1;
      count++;
    }
  }
  if(count != strlen(input->word_))
  {
    input->is_error_ = true;
  }
}

//-----------------------------------------------------------------------------
///
/// Function checks whether the input of the user is correct or not. The word being
/// inserted must not be bigger than the board.
///
/// @param input The word which the user inputed exists in the input struct. Other
/// parameters are used to find position of the word
/// @param config The existing information in the board is compared with what 
/// the user inputed
///
/// @return this function has no return value
//
void check_move_4(Input *input, Config *config)
{
  int i, j = 1;
  char a = input->row_;
  char b = input->column_;
  int c = input->orientation_;
  j = num_of_lines(config->board_);
  i = j * (a - 'a') + b - 'a' + a - 'a';
  if(c == 0)
  {
    if(strlen(input->word_) > j - (b - 'a'))
    {
      input->is_error_ = true;
      return;
    }
  }
  else
  {
    if(strlen(input->word_) > j - (a - 'a'))
    {
      input->is_error_ = true;
      return;
    }
  }
}

//-----------------------------------------------------------------------------
///
/// In case the inputed word is correct this function updates the score of the
/// player which inserted the word
///
/// @param input The word which the user inputed exists in the input struct. Other
/// parameters are used to find position of the word
/// @param config The scores of the letter exist in letter_scores and this compared 
/// with what the user inputed to update the score
///
/// @return this function has no return value
//
void update_score(Input *input, Config *config)
{
  int i;
  int j = 0;
  int score = 0;
  int lines;
  int word_position;
  lines = num_of_lines(config->board_);
  char a = input->row_;
  char b = input->column_;
  int c = input->orientation_;
  word_position = lines * (a - 'a') + b - 'a' + a - 'a';
  for(i = 0; i < strlen(config->letter_score_); i++)
  {
    if(toupper(input->word_[j]) == toupper(config->letter_score_[i]))
    {
      score = score + atoi(&config->letter_score_[i + 1]);
      if(c == 0)
      {
        if(config->board_[word_position] == toupper(input->word_[j]))
        {
          score = score - atoi(&config->letter_score_[i + 1]);
        }
        word_position++;
      }
      else
      {
        if(config->board_[word_position] == toupper(input->word_[j]))
        {
          score = score - atoi(&config->letter_score_[i + 1]);
        }
        word_position = word_position + lines;
        word_position++;
      }
      j++;
      i = -1;
    }
  }
  if(config->player_turn_[0] == '1')
  {
    score = score + atoi(config->player_1_score_);
    sprintf(config->player_1_score_, "%d", score);
    config->player_1_score_[strlen(config->player_1_score_)] = '\n';
  }
  else
  {
    score = score + atoi(config->player_2_score_);
    sprintf(config->player_2_score_, "%d", score);
    config->player_2_score_[strlen(config->player_2_score_)] = '\n';
  }
}

//-----------------------------------------------------------------------------
///
/// Function operates after the word correctness has been checked and it is then 
/// inserted on the board.
///
/// @param input The word which the user inputed exists in the input struct. Other
/// parameters are used to find position of the word.
/// @param config The existing information in the board is compared with what.
/// the user inputed
///
/// @return this function has no return value
//
void put_word_in_board(Input *input, Config *config)
{
  int i = 0;
  int j = 1;
  int word_num;
  char a = input->row_;
  char b = input->column_;
  int c = input->orientation_;
  j = num_of_lines(config->board_);
  config->victory_score_ = j * j / 2;
  i = j * (a - 'a') + b - 'a' + a - 'a';
  check_move(input, config);
  if(input->is_error_ == true)
  {
    return;
  }
  check_move_2(input);
  if(input->is_error_ == true)
  {
    return;
  }
  check_move_3(input, config);
  if(input->is_error_ == true)
  {
    return;
  }
  check_move_4(input, config);
  if(input->is_error_ == true)
  {
    return;
  }
  update_score(input, config);
  for(word_num = 0; word_num < strlen(input->word_); word_num++)
  {
    if(c == 0)
    {
      config->board_[i] = toupper(input->word_[word_num]);
      i++;
    }
    else
    {
      config->board_[i] = toupper(input->word_[word_num]);
      i = i + j;
      i++;
    }
  }
}

//-----------------------------------------------------------------------------
///
/// The file is open in write mode. This function then writes the inhalt of the
/// config struct into the file in the right order
///
/// @param config The information which will be saved in the file is inside the 
/// struct.
/// @param the_file The file in which the information will be written
///
/// @return this function has no return value
//
void save_file(Config *config, FILE *the_file)
{
  int i;
  int j = 0;
  int g = 0;
  int count = 0;
  j = strlen(config->magic_word_);
  for(i = 0; i < strlen(config->board_); i++)
  {
    config->file_[j + i] = config->board_[i];
  }
  g = strlen(config->board_);
  j = j + g;
  int o = 0;
  for(i = 0; i < g; i++)
  {
    if(count == 0)
    {
      config->file_[j + i] = config->player_turn_[o];
      o++;
    }
    else if(count == 1)
    {
      config->file_[j + i] = config->player_1_score_[o];
      o++;
    }
    else if(count == 2)
    {
      config->file_[j + i] = config->player_2_score_[o];
      o++;
    }
    else
    break;
    if(config->file_[j + i] == '\n')
    {
      count++;
      o = 0;
    }
  }
  j = j + i;
  for(o = 0; o < strlen(config->original_letter_score_); o++)
  {
    config->file_[j + o] = config->original_letter_score_[o];
  }
  fprintf(the_file, "%s", config->file_);
}

//------------------------------------------------------------------------------
///
/// The main program.
/// Makes use of all predetermined functions.
/// It starts by reading the configuration file and checks for the magic word.
/// In case the file cannot be read the program end and an error message is printed.
/// Enteres a loop where the user will undergo different operations. The board
/// is printed every time user inserts a word and the program asks the user every time
/// for input. Based the input different functions are called.
/// Program ends when the user inputs command QUIT or CTRL+D.
///
///
/// @return if the config file is not on the right format or it does not exist.
/// @return based on the type of error it exits on different values.
//
int main(int config_file, char *file_config[])
{
  char* user_input;
  Input input;
  FILE *ptr;
  Config config;
  if(config_file != 2)
  {
    printf("Usage: ./a3 configfile\n");
    return 1;
  }
  ptr = fopen(file_config[1],"r");
  if (ptr == NULL)
  {
    printf("Error: Cannot open file: %s\n", file_config[1]);
    return 2;
  }
  read_file(ptr, &config);
  read_magic_word(&config);
  if(strcmp("Scrabble\n", config.magic_word_) != 0)
  {
    printf("Error: Invalid file: %s\n", file_config[1]);
    fclose(ptr);
    free(config.file_);
    free(config.magic_word_);
    return 3;
  }
  read_board(&config);
  read_player_start(&config);
  read_player_score(&config);
  read_letter_score(&config);
  print_board(&config);
  fclose(ptr);
  while(input.command_ != 2 )
  {
    input.is_error_ = false;
    input.only_whitespaces_ = 0;
    user_input = read_user_input();
    parseCommand(user_input, &input, &config);
    if(input.command_ == 3)
    {
      print_help();
      printf("Player %d > ", atoi(config.player_turn_));
    }
    if(input.command_ == 5)
    {
      free(config.original_letter_score_);
      free(config.file_);
      free(config.player_turn_);
      free(config.player_2_score_);
      free(config.player_1_score_);
      free(config.magic_word_);
      free(config.board_);
      free(config.letter_score_);
      ptr = fopen(input.load_config_,"r");
      if (ptr == NULL)
      {
        printf("Error: Cannot open file: %s\n", input.load_config_);
        free(input.load_config_);
        free(user_input);
        free(input.word_);
        return 2;
      }
      free(input.load_config_);
      read_file(ptr, &config);
      read_magic_word(&config);
      read_board(&config);
      read_player_start(&config);
      read_player_score(&config);
      read_letter_score(&config);
      print_board(&config);
      fclose(ptr);
    }
    else if(input.only_whitespaces_ == 1)
    {
      printf("Player %d > ", atoi(config.player_turn_));
    }
    else if(input.is_error_ == 1 && input.command_ == UNKNOWN)
    {
      printf("Error: Unknown command: %s\n", user_input);
      printf("Player %d > ", atoi(config.player_turn_));
    }
    else if(input.is_error_ == 1)
    {
      printf("Error: Insert parameters not valid!\n");
      printf("Player %d > ", atoi(config.player_turn_));
    }
    else if(input.command_ == 2)
    {
      free(user_input);
      free(input.word_);
      break;
    }
    else if(input.command_ == 1)
    {
      ptr = fopen(file_config[1],"w");
      save_file(&config, ptr);
      printf("Player %d > ", atoi(config.player_turn_));
      fclose(ptr);
    }
    else if(input.command_ == 0)
    {
      put_word_in_board(&input, &config);
      if(input.is_error_ == 1)
      {
        printf("Error: Impossible move!\n");
        printf("Player %d > ", atoi(config.player_turn_));
      }
      else if(input.is_error_ == 0)
      {
        if (strcmp("2\n", config.player_turn_) == 0)
        {
          if(atoi(config.player_2_score_) >= config.victory_score_)
          {
            printf("Player 2 has won the game with %d points!\n", atoi(config.player_2_score_));
            free(user_input);
            free(input.word_);
            break;
          }
          config.player_turn_[0] = '1';
        }
        else if(strcmp("1\n", config.player_turn_) == 0)
        {
          if(atoi(config.player_1_score_) >= config.victory_score_)
          {
            printf("Player 1 has won the game with %d points!\n", atoi(config.player_1_score_));
            free(user_input);
            free(input.word_);
            break;
          }
          config.player_turn_[0] = '2';
        }
      print_board(&config);
      }
    }
    free(user_input);
    free(input.word_);
  }
  free(config.original_letter_score_);
  free(config.file_);
  free(config.player_turn_);
  free(config.player_2_score_);
  free(config.player_1_score_);
  free(config.magic_word_);
  free(config.board_);
  free(config.letter_score_);
}
