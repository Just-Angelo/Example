
#include <stdlib.h>
#include<string.h>
#include<stdio.h>

#include "textbuffer.h"
/*
 This is a text buffer ,I made one year ago as a university assignment. I know that there is definitely more efficient ways to do it then the way I did it with a linked list. 
 It doesn't actually take user input from the terminal instead it is just the structure. For a description of what the purpose of each function is you can open textbuffer.h.
 I have attached a test file with it, which you can see the tests or edit as you please. Thanks for reading. 
*/
struct textbuffer {
    	struct string_node *head;
    	struct string_node *last;
    	char *all_text;
};

struct string_node {
    	char *sentence;
    	struct string_node *next;
    	struct string_node *prev;
};


Match new_match(Match match,int lineNumber, int columnNumber);
static void newnode(TB tb);
static void highlighter(TB tb,int line, int from, int to, char highlight);

/**
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */
TB newTB (char *text) {

    	int text_length = 0;
    	text_length = strlen(text);
    	char user_input[text_length + 1];
    	int letter_shift = 0;
    	int input_shift = 0;
    	int input_length = 0;
    	strcpy(user_input, text);
    	input_length = strlen(user_input);
    
    	struct textbuffer *tb = malloc(sizeof(struct textbuffer));
    	tb -> head = NULL;
    	tb -> last = NULL;
    	newnode(tb);
    	struct string_node *current = tb -> head;
    
    	while (input_shift < input_length){
        	current -> sentence = malloc(1);
        	while (user_input[input_shift] != '\n'){
            		current -> sentence[letter_shift] = user_input[input_shift];
            		letter_shift++;
            		input_shift++;
            		current -> sentence = realloc(current -> sentence, letter_shift + 1);
        	}
        	if (user_input[input_shift] == '\n'){
            		current -> sentence = realloc(current -> sentence, letter_shift + 2);
            		current -> sentence[letter_shift] = '\n';
            		current -> sentence[letter_shift + 1] = '\0';
            		newnode(tb);
            		input_shift++;
        	}
        	letter_shift = 0;
        	current = current -> next;
    	}
	return tb;
}

static void newnode(TB tb){
    	struct string_node *new = malloc(sizeof(struct string_node));
    	new -> next = NULL;
    	new -> prev = NULL;
    	if (tb -> head == NULL){
        	tb -> head = new; 
        	tb -> last = new;   
    	}
    	else {
        	tb -> last -> next = new;
        	new -> prev = tb -> last;
        	tb -> last = new;
    	}
} 
Match new_match(Match match_head, int lineNumber, int columnNumber){
    	struct _matchNode *new_match = malloc(sizeof(struct _matchNode));
    	struct _matchNode *current = match_head;
   	new_match -> next = NULL;
    	new_match -> lineNumber = lineNumber;
    	new_match -> columnNumber = columnNumber;
    	if (match_head == NULL){
        	match_head = new_match;
    	}
    	else{
        	while (current -> next != NULL){
            		current = current -> next;
        	}
        	current -> next = new_match;
    	}
    	return match_head;
}

/**
 * Free  the  memory occupied by the given textbuffer. It is an error to
 * access the buffer afterwards.
 */
void releaseTB (TB tb) {
    	struct string_node *current = tb -> head;
    	while (current -> next != NULL){
        	current = current -> next;
        	free(current -> prev -> sentence);
        	free(current -> prev);
    	}
    	free(current);
    	free(tb);
}

/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */
char *dumpTB (TB tb, bool showLineNumbers) {
    	if (showLineNumbers == false){
        	tb -> all_text = malloc(sizeof(char));
        	int letter_count_all = 0;
        	int letter_count_curr = 0;
        	struct string_node *current = tb -> head;
        	if (current == NULL){
            		return NULL;
        	}
        
        	while (current -> next != NULL){
            		while (current -> sentence[letter_count_curr] != '\n'){
               			tb -> all_text[letter_count_all] = current -> sentence[letter_count_curr];
                		letter_count_all++;
                		letter_count_curr++;
                		tb -> all_text = realloc(tb -> all_text, letter_count_all + 1);
            		}
            		if (current -> sentence[letter_count_curr] == '\n'){
                		tb -> all_text[letter_count_all] = '\n';
                		letter_count_all++;
                		letter_count_curr = 0;
                		tb -> all_text = realloc(tb -> all_text, letter_count_all + 1);
            		}
            		current = current -> next;  
        	}
        
        	tb -> all_text[letter_count_all++] = '\0';     
	    	return tb -> all_text;
    	}
    	else {
        	tb -> all_text = malloc(sizeof(char));
        	int letter_count_all = 0;
        	int letter_count_curr = 0;
        	int line_count = 1;
        	int line_copy = 0;
        	int digit_count;
        	char line_no[100];
        	struct string_node *current = tb -> head;
        	      
        	if (current == NULL){
            		return NULL;
        	}
        
        	while (current -> next != NULL){
            		line_copy = line_count;
            		digit_count = 0;
            		while (line_copy > 0){
                		line_no[digit_count++] = '0' + line_copy % 10;
                		line_copy = line_copy/10;               
            		}
            		while (digit_count > 0){
                		tb -> all_text[letter_count_all] = line_no[digit_count - 1];
                		digit_count--; 
                		letter_count_all++;
                		tb -> all_text = realloc(tb -> all_text, letter_count_all + 1);                
            		}
           		tb -> all_text = realloc(tb -> all_text, letter_count_all + 3);
            		tb -> all_text[letter_count_all++] = '.';
            		tb -> all_text[letter_count_all++] = ' ';
            		while (current -> sentence[letter_count_curr] != '\n'){
                		tb -> all_text[letter_count_all] = current -> sentence[letter_count_curr];
                		letter_count_all++;
                		letter_count_curr++;
                		tb -> all_text = realloc(tb -> all_text, letter_count_all + 1);
            		}
            		if (current -> sentence[letter_count_curr] == '\n'){
                		tb -> all_text[letter_count_all] = '\n';
                		letter_count_all++;
                		letter_count_curr = 0;
                		line_count++;
                		tb -> all_text = realloc(tb -> all_text, letter_count_all + 1);
            		}
            		current = current -> next;  
        	}
        	        
        	tb -> all_text[letter_count_all++] = '\0';     
	    	return tb -> all_text;
    	}
}

/**
 * Return the number of lines of the given textbuffer.
 */
int linesTB (TB tb) {
	int line_count = 0;   
    	struct string_node *current = tb -> head;    
    	while (current -> next != NULL){
        	line_count++;
        	current = current -> next;
    	}    
	return line_count;
}

/**
 * Add a given prefix to all lines between 'pos1' and 'pos2', inclusive.
 * - The program should abort() wih an error message if 'pos1' or 'pos2'
 *   is out of range. The first line of a textbuffer is at position 1.
 */
void addPrefixTB (TB tb, int from, int to, char *prefix) {
    	int input_length = strlen(prefix);
    	int sentence_length;
    	int line_count = 1;
    	int sentence_shift = 0;
    	struct string_node *current = tb -> head;    
    	
    	if (from > to){
        	abort();
    	}
    	while (current -> next != NULL){
        	if (line_count >= from && line_count <= to){
            		sentence_length = strlen(current -> sentence);
            		int new_length = sentence_length + input_length;
            		current -> sentence = realloc(current -> sentence, new_length);
            		while (sentence_length > 0){
                		current -> sentence[new_length - 1] = current -> 
                		sentence[sentence_length - 1];
                		sentence_length--;
                		new_length--;
            		}
            		while (sentence_shift < input_length){
                		current -> sentence[sentence_shift] = prefix[sentence_shift];
                		sentence_shift++;
            		}
        	}
        	line_count++;
        	sentence_shift = 0;
        	current = current -> next;
    	}    
    	return;
}

/**
 * Merge 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the merged-in lines from 'tb2'.
 *   - 'tb2' can't be used anymore (as if we had used releaseTB() on it)
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void mergeTB (TB tb1, int pos, TB tb2) {
    	int pos_count = 1;
    	int tb1_lines = linesTB(tb1);
    	int tb2_lines = linesTB(tb2);
    	int nodes_to_make = tb2_lines;
    	char *storage = malloc(1);
    	int char_count = 0;
    	int storage_count = 0;
    	struct string_node *tb1_current = tb1 -> head;
    	struct string_node *tb2_current = tb2 -> head;
    
    	if (pos <= 0 || pos > tb1_lines + 1){
        	abort();
    	}
    	while (nodes_to_make > 0){
        	newnode(tb1);
        	nodes_to_make--;
    	}
    
    	while (pos_count <= tb1_lines + tb2_lines){
        	if (pos_count > tb1_lines && pos_count < pos + tb2_lines){
            		tb1_current -> sentence = malloc((strlen(tb2_current -> sentence) + 1)*sizeof(char));
            		strcpy(tb1_current -> sentence, tb2_current -> sentence);
            		tb2_current = tb2_current -> next;            
        	}
        	else if (pos_count >= pos && pos_count < pos + tb2_lines){
            		while (tb1_current -> sentence[char_count] != '\0'){
                		storage[storage_count] = tb1_current -> sentence[char_count];
                		storage_count++;
                		char_count++;
                		storage = realloc(storage, storage_count + 1);
            		}
            	if (tb1_current -> sentence[char_count] == '\0'){
                	storage[storage_count] = tb1_current -> sentence[char_count];
                	storage_count++;
                	storage = realloc(storage, storage_count + 1);
            	}
            	tb1_current -> sentence = realloc(tb1_current -> sentence ,
            	strlen(tb2_current -> sentence) + 1);
            	strcpy(tb1_current -> sentence, tb2_current -> sentence); 
            	tb2_current = tb2_current -> next;          
        	}
        	else if (pos_count >= pos + tb2_lines && pos_count <= tb1_lines){
            		while (tb1_current -> sentence[char_count] != '\0'){
                    		storage[storage_count] = tb1_current -> sentence[char_count];
                    		storage_count++;
                    		char_count++;
                    		storage = realloc(storage, storage_count + 1);
                	}
                	if (tb1_current -> sentence[char_count] == '\0'){
                    		storage[storage_count] = tb1_current -> sentence[char_count];
                    		storage_count++;
                    		storage = realloc(storage, storage_count + 1);
                	}           
        	}
        	pos_count++;
        	char_count = 0;
        	tb1_current = tb1_current -> next;
    	}
    	tb1_current = tb1 -> head;
    	storage_count = 0;
    	pos_count = 1;
    	while (tb1_current -> next != NULL){
        	if (pos_count >= pos + tb2_lines && pos_count > tb1_lines){
            		tb1_current -> sentence = malloc(1);
            		while (storage[storage_count] != '\0'){
                		tb1_current -> sentence[char_count] = storage[storage_count];
                		char_count++;
                		storage_count++;
                		tb1_current -> sentence = realloc(tb1_current -> sentence, 
                		char_count + 1);            
            		}
            		if (storage[storage_count] == '\0'){
                		tb1_current -> sentence[char_count] = storage[storage_count];
                		storage_count++;
            		}
        	}
        	else if (pos_count >= pos + tb2_lines && pos_count <= tb1_lines){
            		tb1_current -> sentence = realloc(tb1_current -> sentence, 1);
            		while (storage[storage_count] != '\0'){
                		tb1_current -> sentence[char_count] = storage[storage_count];
                		char_count++;
                		storage_count++;
                		tb1_current -> sentence = realloc(tb1_current -> sentence, 
                		char_count + 1);            
            		}
            		if (storage[storage_count] == '\0'){
                		tb1_current -> sentence[char_count] = storage[storage_count];
                		storage_count++;
            		}            
        	}            
        	char_count = 0;
        	pos_count++;
        	tb1_current = tb1_current -> next;
    	}
    	releaseTB(tb2); 
    	free(storage); 
    	return;  
}

/**
 * Copy 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the pasted-in lines from 'tb2'.
 *   - 'tb2' is unmodified and remains usable independent of tb1.
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void pasteTB (TB tb1, int pos, TB tb2) {

    	int pos_count = 1;
    	int tb1_lines = linesTB(tb1);
    	int tb2_lines = linesTB(tb2);
    	int nodes_to_make = tb2_lines;
    	char *storage = malloc(1);
    	int char_count = 0;
    	int storage_count = 0;
    	struct string_node *tb1_current = tb1 -> head;
    	struct string_node *tb2_current = tb2 -> head;
    
    	if (pos <= 0 || pos > tb1_lines + 1){
        	abort();
    	}
    	while (nodes_to_make > 0){
        	newnode(tb1);
        	nodes_to_make--;
    	}
    
    	while (pos_count <= tb1_lines + tb2_lines){    	
        	if (pos_count > tb1_lines && pos_count < pos + tb2_lines){
            		tb1_current -> sentence = malloc((strlen(tb2_current -> sentence) + 1)*sizeof(char));
            		strcpy(tb1_current -> sentence, tb2_current -> sentence);
            		tb2_current = tb2_current -> next;            
        	}
        	else if (pos_count >= pos && pos_count < pos + tb2_lines){
            		while (tb1_current -> sentence[char_count] != '\0'){
                		storage[storage_count] = tb1_current -> sentence[char_count];
                		storage_count++;
                		char_count++;
                		storage = realloc(storage, storage_count + 1);
            		}
            		if (tb1_current -> sentence[char_count] == '\0'){
                		storage[storage_count] = tb1_current -> sentence[char_count];
                		storage_count++;
                		storage = realloc(storage, storage_count + 1);
            		}
            		tb1_current -> sentence = realloc(tb1_current -> sentence ,
            		strlen(tb2_current -> sentence) + 1);
            		strcpy(tb1_current -> sentence, tb2_current -> sentence); 
            		tb2_current = tb2_current -> next;          
        	}
        	else if (pos_count >= pos + tb2_lines && pos_count <= tb1_lines){
            		while (tb1_current -> sentence[char_count] != '\0'){
                    		storage[storage_count] = tb1_current -> sentence[char_count];
                    		storage_count++;
                    		char_count++;
                    		storage = realloc(storage, storage_count + 1);
                	}
                	if (tb1_current -> sentence[char_count] == '\0'){
                    		storage[storage_count] = tb1_current -> sentence[char_count];
                    		storage_count++;
                    		storage = realloc(storage, storage_count + 1);
                	}           
        	}
        	pos_count++;
        	char_count = 0;
        	tb1_current = tb1_current -> next;
    	}
    	tb1_current = tb1 -> head;
    	storage_count = 0;
    	pos_count = 1;
    	while (tb1_current -> next != NULL){
        	if (pos_count >= pos + tb2_lines && pos_count > tb1_lines){
            		tb1_current -> sentence = malloc(1);
            		while (storage[storage_count] != '\0'){
                		tb1_current -> sentence[char_count] = storage[storage_count];
                		char_count++;
                		storage_count++;
                		tb1_current -> sentence = realloc(tb1_current -> sentence, 
                		char_count + 1);            
            		}
            		if (storage[storage_count] == '\0'){
                		tb1_current -> sentence[char_count] = storage[storage_count];
                		storage_count++;
            		}
        	}
        	else if (pos_count >= pos + tb2_lines && pos_count <= tb1_lines){
            		tb1_current -> sentence = realloc(tb1_current -> sentence, 1);
            		while (storage[storage_count] != '\0'){
                		tb1_current -> sentence[char_count] = storage[storage_count];
                		char_count++;
                		storage_count++;
                		tb1_current -> sentence = realloc(tb1_current -> sentence, 
                		char_count + 1);            
            		}
            		if (storage[storage_count] == '\0'){
                		tb1_current -> sentence[char_count] = storage[storage_count];
                		storage_count++;
            		}            
        	}            
        	char_count = 0;
        	pos_count++;
        	tb1_current = tb1_current -> next;
    	}
    	free(storage); 
}

/**
 * Cut  the lines between and including 'from' and 'to' out of the given
 * textbuffer 'tb' into a new textbuffer.
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
TB cutTB (TB tb, int from, int to) {
    	char *removed_text = malloc(1);
    	int letter_count_all = 0;
    	int letter_count_curr = 0;
    	int line_count = 1;
    	struct string_node *current = tb -> head;
    	if (current == NULL){
        	abort();
    	}
    	if (from > to || to <= 0){
        	abort();
    	}    
    	while (current -> next != NULL){
        	if (line_count >= from && line_count <= to){
            		while (current -> sentence[letter_count_curr] != '\n'){
                		removed_text[letter_count_all] = current -> sentence[letter_count_curr];
                		letter_count_all++;
                		letter_count_curr++;
                		removed_text = realloc(removed_text, letter_count_all + 1);    
            		}
            		if (current -> sentence[letter_count_curr] == '\n'){
                		removed_text[letter_count_all] = current -> sentence[letter_count_curr];
                		letter_count_all++;
                		removed_text = realloc(removed_text, letter_count_all + 1);    
            		}
        	}
        	letter_count_curr = 0;
        	line_count++;
        	current = current -> next;
    	}
    	removed_text[letter_count_all] = '\0';
    	deleteTB(tb, from, to);
    	TB tb_cut = newTB(removed_text);
    	free(removed_text);    
	return tb_cut;
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB (TB tb, char *search) {
    	int line_count = 1;
    	int pos_count = 1;
    	int search_length = strlen(search);
    	int sent_length;
    	int match_count = 0;
    	int sent_shift = 0;
    	int search_shift = 0;
    	struct _matchNode *match_head;
    	match_head = NULL;
    
   	struct string_node *current = tb -> head;
    	if (current == NULL){
        	return match_head;
    	}
    	else{
        	while (current -> next != NULL){
            		sent_length = strlen(current -> sentence);
            		while (sent_shift < sent_length){
                		if (current -> sentence[sent_shift] == search[search_shift]){
                    			match_count++;
                   	 		sent_shift++;
                    			search_shift++;
                    			pos_count++;
                		}
                		else if (current -> sentence[sent_shift] != search[search_shift]){
                    			sent_shift++;
                    			match_count = 0;
                    			search_shift = 0;
                    			pos_count++;
                		}
                		if (match_count == search_length){
                    			match_head = new_match(match_head, line_count, pos_count - search_length);
                    			match_count = 0;
                    			search_shift = 0;
                		}	           
            		}            
            		current = current -> next;
            		line_count++;
            		pos_count = 1;
            		match_count = 0;
            		sent_shift = 0;
            		search_shift = 0;
        	}    
    	}        
	return match_head;
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
void deleteTB (TB tb, int from, int to) {
    	int line_count = 1;
   	struct string_node *current = tb -> head;
   	    
    	if (to < from){
        	abort();
    	}
    
   	while (current -> next != NULL){
        	if (line_count >= from && line_count <= to){
            		if (current == tb -> head){
                		tb -> head = current -> next;
                		current = current -> next;
                		free(current -> prev -> sentence);
                		free(current -> prev);
                		current -> prev = NULL;               
            		}
            	else if (current != tb -> head){
                	struct string_node *copy = current;
                	current -> prev -> next = current -> next;
                	current -> next -> prev = current -> prev;
                	current = current -> next;
                	free(copy -> sentence);
                	free(copy);                
            	}
            	line_count++;
        	}
        	else{
            		line_count++;
            		current = current -> next;
        	}
    	}
}

/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText (TB tb) {
	int pos_count = 1;
	int pos_count_end = 1;
	int sentence_length;
	int length_count = 0;
	char *storage = malloc(1);
	int line_count = 1;
	struct string_node *current = tb -> head;

	if (current == NULL){
    		return;
	}

	while (current -> next != NULL){
    		sentence_length = strlen(current -> sentence);
    		length_count = 0;
    		pos_count = 0;
    		storage = realloc(storage, sentence_length + 1);
    		strcpy(storage, current -> sentence);           
    		if (current -> sentence[0] == '#' && sentence_length > 1){
        		current -> sentence = realloc(current -> sentence, sentence_length + 9);
        		current -> sentence [0] = '<';
        		current -> sentence [1] = 'h';
        		current -> sentence [2] = '1';
        		current -> sentence [3] = '>';
        		current -> sentence [sentence_length + 8] = '\0';
        		current -> sentence [sentence_length + 7] = '\n';
        		current -> sentence [sentence_length + 6] = '>';
        		current -> sentence [sentence_length + 5] = '1';
        		current -> sentence [sentence_length + 4] = 'h';
        		current -> sentence [sentence_length + 3] = '/';
        		current -> sentence [sentence_length + 2] = '<';
        		length_count = 1;
              
        		while (length_count < sentence_length - 1){
            			current -> sentence [length_count + 3] = storage[length_count];
            			length_count++;
        		}            
    		}
    	else{        
        	while (pos_count < sentence_length){
            		pos_count_end = strlen(current -> sentence);           
            		while (pos_count_end > pos_count + 1 && pos_count_end <= sentence_length){
                		if (storage[pos_count] == storage[pos_count_end] &&
                    			storage[pos_count] == '*'){
                    			highlighter(tb,line_count,pos_count + 1,pos_count_end + 1,'b');
                    			pos_count = pos_count_end + 1;                 
                		}		
                		if (storage[pos_count] == storage[pos_count_end] &&
                    			storage[pos_count] == '_'){
                    			highlighter(tb,line_count,pos_count + 1,pos_count_end + 1,'i');
                    			pos_count = pos_count_end + 1;                
                		}                           
                		pos_count_end--;
            		}
            		sentence_length = strlen(current -> sentence);
            		storage = realloc(storage, sentence_length + 1);
            		strcpy(storage, current -> sentence);
            		pos_count++;
        	}      
    	}
    	line_count++;
    	current = current -> next;
	}
    	free(storage);
}

static void highlighter(TB tb,int line, int from, int to, char highlight){

    	int line_count = 1;
    	int pos_count = 0;
    	int storage_count = 0;
    	int sentence_length;
    	char *storage = malloc(1);
    	struct string_node *current = tb -> head;
    
    	if (to == from + 1){
        	return;
    	}
    	while (current -> next != NULL){
        	sentence_length = strlen(current -> sentence);
        	storage = realloc (storage, sentence_length + 1);
        	strcpy(storage, current -> sentence);
        	if  (line_count == line){
            		current -> sentence = realloc( current -> sentence , sentence_length + 6);
            		while (pos_count < sentence_length + 6){            
                		if (pos_count == from - 1){
                    			current -> sentence[pos_count] = '<';
                    			current -> sentence[pos_count + 1] = highlight;
                    			current -> sentence[pos_count + 2] = '>';
                   	 		pos_count = pos_count + 3;
                    			storage_count++;                    
               			}
                	else if (pos_count == to + 1){
                    		current -> sentence[pos_count] = '<';
                    		current -> sentence[pos_count + 1] = '/';
                    		current -> sentence[pos_count + 2] = highlight;
                    		current -> sentence[pos_count + 3] = '>';
                    		pos_count = pos_count + 4;
                    		storage_count++;
                	}
                		current -> sentence[pos_count] = storage[storage_count];
                		pos_count++;
                		storage_count++;
            		}
            		current -> sentence[sentence_length + 5] = '\0';                                       
        	}
        	line_count++;
        	current = current -> next;
    	}
    	free(storage);
    	return;
}



