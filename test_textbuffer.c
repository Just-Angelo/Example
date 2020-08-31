// testTextbuffer.c
// A stub file for you to test your textbuffer implementation.
// Note that you will NOT be submitting this - this is simply for you to
// test  your  functions as you go. We will have our own testTexbuffer.c
// for testing your submission.

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

static void testNewTB(void);
static void testAddPrefixTB(void);
static void testMergeTB(void);
static void testPasteTB(void);
static void testCutTB(void);
static void testSearchTB(void);
static void testFormRichText(void);
static void releaseMatch(Match match);


int main(void) {
	testNewTB();
	testAddPrefixTB();
	testMergeTB();
	testPasteTB();
	testCutTB();
	testSearchTB();
	testFormRichText();
	// TODO: Call more test functions
	
	printf("\nAll tests passed! You are awesome!\n");
}

static void testNewTB(void) {
	printf("-----------------------------------------\n"
	       "               newTB tests               \n"
	       "-----------------------------------------\n");
	
	// Calling dumpTB immediately after newTB, without modifying the TB
	TB tb1 = newTB("hello there,\nhow\nare\nthings\nI\nam\nwell\n");
	char *text1 = dumpTB(tb1, true); // show line numbers
	assert(strcmp("1. hello there,\n2. how\n3. are\n4. things\n5. I\n6. am\n7. well\n", text1) == 0);
	assert(linesTB(tb1) == 7);
	free(text1);

	char *text2 = dumpTB(tb1,false);	
	assert(strcmp("hello there,\nhow\nare\nthings\nI\nam\nwell\n",text2) == 0);
	free(text2);
	releaseTB(tb1);

	/*tb1 = newTB("");
	assert(dumpTB(tb1,false) == NULL);
	assert(dumpTB(tb1,true) == NULL);
	assert(linesTB(tb1) == 0);
	releaseTB(tb1);*/

	tb1 = newTB("\n");
	char *text3 = dumpTB(tb1,false);
	char *text3a = dumpTB(tb1,true);
	assert(strcmp(text3,"\n") == 0);
	assert(strcmp(text3a, "1. \n") == 0);
	assert(linesTB(tb1) == 1);
	releaseTB(tb1);
	free(text3);
	free(text3a);

	tb1 = newTB("\n\n\n\n\n\n\n\n");
	char *text4 = dumpTB(tb1,false);
	char *text4a = dumpTB(tb1,true);
	assert(strcmp(text4,"\n\n\n\n\n\n\n\n") == 0);
	assert(strcmp(text4a,"1. \n2. \n3. \n4. \n5. \n6. \n7. \n8. \n") == 0);
	assert(linesTB(tb1) == 8);
	releaseTB(tb1);
	free(text4);
	free(text4a);

	
	printf("newTB tests passed!\n");
}
static void testAddPrefixTB(void){
	printf("-----------------------------------------\n"
	       "            addPrefixTB tests            \n"
	       "-----------------------------------------\n");
	TB tb1 = newTB("ring to rule them all\n"
					"ring to find them\n"
					"ring to bring them all\n"
					"And in the darkness bind them\n"
					"In the land of Mordor where the shadows lie\n");
	addPrefixTB(tb1,1,3, "One ");
	char *text = dumpTB(tb1,false);
	assert(strcmp(text, "One ring to rule them all\n"
						"One ring to find them\n"
						"One ring to bring them all\n"
						"And in the darkness bind them\n"
						"In the land of Mordor where the shadows lie\n"
						) == 0);	
	free(text);
	releaseTB(tb1);

	tb1 = newTB("\n\n\n");
	addPrefixTB(tb1,1,1,"Hi");
	text = dumpTB(tb1,false);
	assert(strcmp(text,"Hi\n\n\n") == 0);
	free(text);
	releaseTB(tb1);

	tb1 = newTB("Hello I am Ming\nWhat is your name?\n");
	addPrefixTB(tb1,1,2," ");
	text = dumpTB(tb1,false);
	assert(strcmp(text," Hello I am Ming\n What is your name?\n") == 0);
	free(text);
	releaseTB(tb1);
	printf("addPrefixTB tests passed!\n");
}
static void testMergeTB(void){
	printf("-----------------------------------------\n"
	       "             mergeTB tests               \n"
	       "-----------------------------------------\n");
	TB tb1 = newTB("One ring to rule them all\n"
					"In the land of Mordor where the shadows lie\n");
	TB tb2 = newTB("One ring to find them\n"
					"One ring to bring them all\n"
					"And in the darkness bind them\n");
	mergeTB(tb1,2,tb2);
	char *text = dumpTB(tb1,false);
	assert(strcmp(text,"One ring to rule them all\n"
						"One ring to find them\n"
						"One ring to bring them all\n"
						"And in the darkness bind them\n" 
						"In the land of Mordor where the shadows lie\n") == 0);
	free(text);
	releaseTB(tb1);

	tb1 = newTB("One ring to rule them all\n"
					"In the land of Mordor where the shadows lie\n");
	tb2 = newTB("\n\n\n");
	mergeTB(tb1,2,tb2);
	text = dumpTB(tb1,false);
	assert(strcmp(text,"One ring to rule them all\n\n\n\n"
						"In the land of Mordor where the shadows lie\n") == 0);
	free(text);
	releaseTB(tb1);

	printf("mergeTB tests passed!\n");
}
static void testPasteTB(void){
	printf("-----------------------------------------\n"
	       "             pasteTB tests               \n"
	       "-----------------------------------------\n");

	TB tb1 = newTB("One ring to rule them all\n"
					"In the land of Mordor where the shadows lie\n");
	TB tb2 = newTB("One ring to find them\n"
					"One ring to bring them all\n"
					"And in the darkness bind them\n");
	pasteTB(tb1,2,tb2);
	char *text = dumpTB(tb1,false);
	assert(strcmp(text,"One ring to rule them all\n"
						"One ring to find them\n"
						"One ring to bring them all\n"
						"And in the darkness bind them\n" 
						"In the land of Mordor where the shadows lie\n") == 0);
	free(text);
	releaseTB(tb1);
	releaseTB(tb2);

	tb1 = newTB("One ring to rule them all\n"
					"In the land of Mordor where the shadows lie\n");
	tb2 = newTB("\n\n\n");
	pasteTB(tb1,2,tb2);
	text = dumpTB(tb1,false);
	assert(strcmp(text,"One ring to rule them all\n\n\n\n"
						"In the land of Mordor where the shadows lie\n") == 0);
	free(text);
	releaseTB(tb1);
	releaseTB(tb2);



	printf("pasteTB tests passed!\n");

}
static void testCutTB(void){
	printf("-----------------------------------------\n"
	       "             cutTB tests               \n"
	       "-----------------------------------------\n");

	TB tb1 = newTB("One ring to rule them all\n"
					"One ring to find them\n"
					"One ring to bring them all\n"
					"And in the darkness bind them\n"
					"In the land of Mordor where the shadows lie\n");
	TB tb2 = cutTB(tb1,2,4);
	char *text2 = dumpTB(tb2,false);
	char *text = dumpTB(tb1,false);
	assert(strcmp(text,"One ring to rule them all\n""In the land of Mordor where the shadows lie\n") == 0);
	assert(strcmp(text2,"One ring to find them\n"
					"One ring to bring them all\n"
					"And in the darkness bind them\n") == 0);
	releaseTB(tb2);
	free(text);
	free(text2);
	releaseTB(tb1);





	tb1 = newTB("One ring to rule them all\n"
					"One ring to find them\n"
					"One ring to bring them all\n"
					"And in the darkness bind them\n"
					"In the land of Mordor where the shadows lie\n");

	tb2 = cutTB(tb1,2,5);
	text = dumpTB(tb1,false);
	text2 = dumpTB(tb2,false);
	assert(strcmp(text,"One ring to rule them all\n") == 0);
	assert(strcmp(text2,"One ring to find them\n"
					"One ring to bring them all\n"
					"And in the darkness bind them\n"
					"In the land of Mordor where the shadows lie\n") == 0);
	free(text);
	free(text2);
	releaseTB(tb1);
	releaseTB(tb2);

	

	tb1 = newTB("\n\n\n\nlel\n\n\n\n\n\n"); //10 newlines
	tb2 = cutTB(tb1,1,5);
	text = dumpTB(tb1,false);
	text2 = dumpTB(tb2,false);
	assert(strcmp(text,"\n\n\n\n\n") == 0);
	assert(strcmp(text2,"\n\n\n\nlel\n") == 0);
	releaseTB(tb1);
	releaseTB(tb2);
	free(text);
	free(text2);

	tb1 = newTB("\n\n\n\nlel\n\n\n\n\n\n"); //10 newlines
	tb2 = cutTB(tb1,1,1);
	text = dumpTB(tb1,false);
	text2 = dumpTB(tb2,false);
	assert(strcmp(text,"\n\n\nlel\n\n\n\n\n\n") == 0);
	assert(strcmp(text2,"\n") == 0);
	releaseTB(tb1);
	releaseTB(tb2);
	free(text);
	free(text2);

	printf("cutTB tests passed!\n");
}
static void testSearchTB(void){
	printf("-----------------------------------------\n"
	       "            searchTB tests               \n"
	       "-----------------------------------------\n");

	TB tb1 = newTB("Three Rings for the Elven-kings under the sky,\n"
					"Seven for the Dwarf-lords in their halls of stone,\n"
					"Nine for Mortal Men, doomed to die,\n"
					"One for the Dark Lord on his dark throne\n"
					"In the Land of Mordor where the Shadows lie.\n");
	Match find = searchTB(tb1,"for");
	
	Match cur = find;
	if(find == NULL){
		printf("No matches!\n");
		releaseTB(tb1);
		return;
	}
	
	int numFinds = 0;
	while(cur != NULL){
		printf("Column %d on line %d\n",cur->columnNumber,cur->lineNumber);
		numFinds++;
		cur = cur->next;
	}
	releaseTB(tb1);
	
	releaseMatch(find);
	printf("cutTB tests passed!\n");
	return;
}


static void testFormRichText(void){
	printf("-----------------------------------------\n"
	       "            FormRichTextTB tests         \n"
	       "-----------------------------------------\n");

	TB tb1 = newTB("#Three Rings for the Elven-kings under the sky\n"
					"Seven for the _Dwarf-lords_ in their halls of stone\n"
					"Nine for mortal *Men*, doomed to die\n"
					"One for the _Dark Lord_ on his dark throne\n");	
	formRichText(tb1);
	char *text = dumpTB(tb1,false);
	assert(strcmp(text, "<h1>Three Rings for the Elven-kings under the sky</h1>\n"
					"Seven for the <i>Dwarf-lords</i> in their halls of stone\n"
					"Nine for mortal <b>Men</b>, doomed to die\n"
					"One for the <i>Dark Lord</i> on his dark throne\n") == 0);
	free(text);
	releaseTB(tb1);

	printf("FormRichTextTB tests passed!\n");
}


static void releaseMatch(Match match){
	Match temp;
	Match cur = match;
	while(cur != NULL){
		temp = cur->next;
		free(cur);
		cur = temp;
	}
	return;

}
// TODO: Add more test functions here

