#!/usr/bin/perl -w
=pod
Header:
Created at 03/08/2020 by Angelo Zhou (Z5255835). This program works by reading
the shell program and putting the lines into an array. After this each line is
filtered for key words and if detected will be edited accordingly to translate
from shell to perl.

Note: 
This is a very basic shell to perl translator. It works for basic echos, for loops if, else ,elif and whiles. It does some indentation however this has errors with nesting.
It also misses many edge cases. This was one of my university assignments. I have attached some demo shell code which it translates successfully.
=cut
# opens shell file and counts how many lines are in it.
$file = $ARGV[0];
open F , '<' , $file or die;
$line_count = 0;

while ($input = <F>) {
    push @lines, $input;
    $line_count++;
}
close F;
# This line should always exist if its a non-empty valid shell file.
if ($line_count > 0) {
    	@lines[0] = "#!/usr/bin/perl -w"
}
$line_count = 0;
# We loop through each line and search for the key words and then use the appropiate function.
foreach $line (@lines) {
    # If theres a # and theres no echo there is commenting to do. (I know there is $# but for the
    # purpose of simplification we shall assume $# wont be dealt with.) 
    
    	if ($line =~ /\#/ and $line !~ /echo '.*#.*'/ and $line ne "#!/usr/bin/perl -w") {
        	chomp($line);
        	$line = hash_tag($line);
        	push @lines_to_comment, $line_count;
    	}
    # If we encounter an echo ,we call the echo function.
    	if ($line =~ /echo/) {
        	chomp($line);
        	$line = echo($line);              
    	}
    #some simple functions done through system.    
    	elsif ($line =~ /ls/ and $line !~ /else/) {
        	$hold = $line;
        	chomp ($hold);
        	$line = "system \"$hold\";";
    	}
    	elsif ($line =~ /pwd/) {
        	$hold = $line;
        	chomp ($hold);
        	$line = "system \"$hold\";";
    	}
    	elsif ($line =~ /id/) {
        	$hold = $line;
        	chomp ($hold);
        	$line = "system \"$hold\";";
    	}
    	elsif ($line =~ /date/) {
        	$hold = $line;
        	chomp ($hold);
        	$line = "system \"$hold\";";
    	}
    # If theres a "=" and no test it is assumed as an assignment like a=5, keep
    # in mind this is a very basic shell to perl converter.
    	elsif ($line =~ /=/ and $line !~ /.*test.*/) {
        	@equals = split("=", $line);
        	chomp ($equals[1]);
        	chomp($line);
        	$line =~ s/^\s+//;
        	$line =~ s/=/ = /;
        	$line =~ s/$equals[1]/\'$equals[1]\'/;
        	$line =~ s/$equals[0]/\$$equals[0]/;
        	@equals = ();
        	$line = "$line\;";                
    	}
    # Here we just convert cd to chdir and put brackets around the arguement after.
    # i.e cd /tmp to chdir '/tmp'.
    	elsif ($line =~ /cd/) {
        	@equals = split (" ", $line);
        	chomp ($equals[1]);
        	chomp($line);
        	$line =~ s/cd/chdir/;
        	$line =~ s/$equals[1]/'$equals[1]'/;
        	@equals = ();
        	$line = "$line\;";       
    	}
    # done doesn't exist in perl instead its replaced by }.
    	elsif ($line eq "done\n") {
        	$line = "}";
    	}
    # Here we take it to our function that edits for statements. 
    	elsif ($line =~ /for/) {
        	chomp($line);
        	$line = replacement_for($line);        
    	}
    # Get rid of white space and minor edits.
    	elsif ($line =~ /exit/) {
        	chomp($line);
        	$line = "$line\;";
        	$line =~ s/^\s+//;
    	}
    # Remove white spaces and some string manipulation.
    	elsif ($line =~ /read/) {
        	chomp($line);
        	$line =~ s/^\s+//;
        	@line_split = split(" ", $line);
        	$var = $line_split[1];
        	@line_split = ();
        	$line = "\$$var = <STDIN>\;\nchomp \$$var\;"
    	}
    # Just sends to appropiate functions.
    	elsif ($line =~ /if test.*/ and $line !~ /elif .*/) {
        	$line = if_statement($line);   
    	}
    	elsif ($line eq "fi\n") {
        	$line = "\}";
    	}
    	elsif ($line eq "else\n") {
        	$line = "\} else \{";
    	}
    	elsif ($line =~ /elif test.*/) {
        	$line = elif_statement($line);
    	}
    	elsif ($line =~ /while/) {
        	$line = while_statement($line);
    	}
    	$line_count++;
}
=pod
line_count simply counts what line we are on and line_comment is an index for
comments stored in the comments array.
=cut
$line_count = 0;
$line_comment = 0;

foreach $line (@lines) {
    chomp($line);
    # @to_edit is an array which holds expressions after "in" i.e for file in *.c
    # we then edit to the perl equivalent with glob.
    	foreach $edit (@to_edit) {
        	if ($line =~ /.*foreach.*/ and $line =~ /.*\\$edit.*/) {
            		$line =~ s/\(.*\)/\(glob\("$edit"\)\)/;
        	}  
    	}
    # Simply converts arguments like $1 to perl equivalent.
    	if ($line =~ /.*\$[0-9].*/){
        	$line = arg_replace($line);        
    	}
    # Converts some operators to perl equivalent.
    	$line =~ s/\-eq/==/;
    	$line =~ s/\-ne/!=/;
    	$line =~ s/\-lt/</;
    	$line =~ s/\-le/<=/;
    	$line =~ s/\-gt/>/;
    	$line =~ s/\-ge/>=/;
    #converts expr to perl equivalent.
    	if ($line =~ /expr/ and $line =~ /=/) {
     		$line = expre($line);       
    	}
    # @lines to comment is an array which holds all lines to commeny, we just make sure
    # to add the comments if the current line is one which is supposed to be commented.
    	foreach $comment (@lines_to_comment) {
        	if ($comment == $line_count) {
            		if ($line eq "") {
                		$line = $comments[$line_comment];
            		}
            		else {
                		$line = "$line\t$comments[$line_comment]";
            		}
            	$line_comment++;  
        	}
    	}
    	$line_count++;    
}
# This is a indentation function basically works by adding an indent for { and
# reducing indent for } and then prints the line. It also deals with some exceptions.
$elsif_indent = 0;
$indent_count = 0;
foreach $line (@lines) {
    	$line_indent = 0;
    
    	if ($line =~ /\}.*elsif.*\{/ and $elsif_indent == 1) {
        	$elsif_indent = 0;
        	$indent_count--; 
    	} 
    	if ($line =~ /.*\}/ and $line !~ /\}.*\{/) {
        	$indent_count--;   
    	}
    	if ($line =~ /\}.*else.*\{/) {
        	$indent_count--;
    	}
    
    	if ($line ne "do" and $line !~ /.*<STDIN>.*/ and $line ne "then") {
        	while ($line_indent < $indent_count) {
            		printf "\t";
            		$line_indent++;
        	}
        	printf "$line\n";
    	}
    
    	if ($line =~ /.*<STDIN>.*/ ) {
        	@split = split("\n", $line);
        	while ($line_indent < $indent_count) {
            		printf "\t";
            		$line_indent++;
       		}
        	printf "$split[0]\n";
        	$line_indent = 0;
        	while ($line_indent < $indent_count) {
            		printf "\t";
            		$line_indent++;
        	}
        	printf "$split[1]\n";
        	@split = ();          
    	}
    
    	if ($line =~ /.*\{/ and $line !~ /\}.*\{/) {
        	$indent_count++;   
    	}
    	elsif ($line =~ /\}.*elsif.*\{/ and $elsif_indent == 0) {
        	$elsif_indent = 1;
        	$indent_count++;     
    	}     
    	if ($line =~ /\}.*else.*\{/) {
        	$indent_count++;
    	}
    
}
# A function which uses string manipulation to change for statements into perl.
sub replacement_for{
    	$to_replace = $_[0];
    	@to_replace_split = split(" ", $to_replace);
    	$index_count = 0;
    	$word_in_brack = 0;
    	$stored_words = 0;
    # This loop puts $ in front of variables, replaces for with foreach and lastly
    # puts words into to edit if there is a glob equivalent.
    	foreach $element (@to_replace_split) {
        	if ($element eq "for") {
            		$element = "foreach ";
        	}
        	elsif ($element ne "for" and $index_count > 0 and $to_replace_split[$index_count - 1] eq "foreach ") {
            		$element = "\$$element ";    
        	}
        	elsif ($element eq "in") { 
            		$word_in_brack = 1;    
        	}
        	elsif ($word_in_brack == 1) {
            		if ($element =~ /\*/) {
               			push @words, "\\$element";
                		push @to_edit, "$element"; 
                		$stored_words++; 
            		} 
            		else {
                		push @words, $element;  
                		$stored_words++; 
            		}  
        	}
        	$index_count++;
    	}
    # some string/array manipulation to edit brackets and what not.
    	$replacement = join("", @to_replace_split);
    	$replacement =~ s/in//;
    	$word_combined = join("", @words);
    	$replacement =~ s/$word_combined/\($word_combined\) \{/;
    	$replacement =~ s/\\\*/\*/ ;
    	foreach $word (@words){
        	if ( $word ne $words[$stored_words - 1] ) {
            		if ( $word =~ /.*[0-9].*/ ){
                		$replacement =~ s/$word/$word\, /;
            		}
            		elsif ( $word =~ /.*[a-z A-Z].*/ ) {
                		$replacement =~ s/$word/\'$word\'\, /;
            		}
        	}
        	elsif ( $word eq $words[$stored_words - 1] ) {
            		if ( $word =~ /.*[a-z A-Z].*/ ) {
                		$replacement =~ s/$word/\'$word\'/;
            		}
        	}
    	}
    	@to_replace_split = ();
    	return $replacement;    
}
# Just changes $1 to $ARGV[1] or any other $[0-9]
sub arg_replace {
    	$to_replace = $_[0];
    	@split = split("", $line);
    	$index_count = 0;
    	$replacement = $to_replace;
    	foreach $element (@split) {
        	if ($index_count > 1 and $split[$index_count - 2] eq "\\") {
            	# we just do nothing since it escapes
        	}
        	elsif ($index_count > 0 and $split[$index_count - 1] eq "\$" and $element =~ /[0-9]/) {
            		$index_change = $element - 1;
            		$replacement =~ s/\$$element/\$ARGV\[$index_change\]/;
        	}                                                           
        	$index_count++;
    	}
    	@split = ();
    	return $replacement;   
}
# just shifts some brackets and change some operators to their perl coutner parts.
sub if_statement{
    	$to_replace = $_[0];
    	@split = split("", $to_replace);
    	$index_count = 0;    
    	for $characters (@split) {
        	if ($index_count > 0 and $split[$index_count - 1] eq "=") {
            		if ($characters =~ /[0-9]/) {
               			@split[$index_count - 1] = "==";
           		}
            		else {
               			@split[$index_count - 1] = "eq";
            		} 
        	}
        	$index_count++;   
    	}
    	@split_2 = split(" ", $to_replace);
    	$index_count = 0;
    	for $character (@split_2) {
        	if ( $character eq "=" ) {
            		push @key_words, $split_2[$index_count - 1];
            		push @key_words, $split_2[$index_count + 1];
        	}     
        	$index_count++;
   	 }
    	$replacement = join("", @split);
    	@split = ();
    	@split_2 = ();
    	$replacement =~ s/ test //;
    	$replacement =~ s/if/if \(/;
    	$replacement =~s/\n/\) \{/;
    
    	foreach $word (@key_words) {
        	$replacement =~ s/$word/\'$word\'/;
    	}
    	@key_words = ();
    	return $replacement;
}
# Ditto.
sub elif_statement{
	$to_replace = $_[0];
    	@split = split("", $to_replace);
    	$index_count = 0;
    
    	for $characters (@split) {
        	if ($index_count > 0 and $split[$index_count - 1] eq "=") {
            		if ($characters =~ /[0-9]/) {
               			@split[$index_count - 1] = "==";
            		}
            		else {
               			@split[$index_count - 1] = "eq";
            		}  
        	}
        	$index_count++;   
    	}
    	@split_2 = split(" ", $to_replace);
    	$index_count = 0;
    	for $character (@split_2) {
        	if ($character eq "=") {
            		push @key_words, $split_2[$index_count - 1];
            		push @key_words, $split_2[$index_count + 1];
        	}     
        	$index_count++;
    	}
   	$replacement = join("", @split);
    	@split = ();
    	@split_2 = ();
    	$replacement =~ s/ test //;
    	$replacement =~ s/elif/\} elsif \(/;
    	$replacement =~s/\n/\) \{/;        
    	foreach $word (@key_words) {
        	$replacement =~ s/$word/\'$word\'/;
    	}
    	@key_words = ();
    	return $replacement;
}
# This works by reading everything after echo but before the new line then putting
# the line into print format. However it also checks for brackets in shell and if 
# brackets are detected it changes it accordingly.
sub echo{
	$line = $_[0];
    	$index_echo = index ($line, "echo");
    	$read_from = $index_echo + 4;
    	@characters = split("", $line);        
    	$index_count = 0;
    	$first_char = $index_echo + 5;
    	$index_end = length($line) - 1;       
        foreach $index (@characters) {
        	if ($characters [$first_char] eq "\'" and $characters [$index_end] eq "\'"
            	and $index_count > $read_from and $index_count < $index_end) {
                	if ($index_count != $first_char and $index_count != $index_end) {
                   		 if ($index eq "\"" or $index eq "\'" or $index eq "\;" or $index eq "\$") {
                        		push @word, "\\$index";
                    		}
                    		else {
                        		push @word, $index;
                    		}  
                	}
           	}  
            	elsif ($characters [$first_char] eq "\"" and $characters [$index_end] eq "\""
            	and $index_count > $read_from and $index_count < $index_end) { 
                	if ($index_count != $first_char and $index_count != $index_end) {
                    		if ($index eq "\"" or $index eq "\'" or $index eq "\;" ) {                     
                        		push @word, "\\$index";
                    		}
                    		else {
                        		push @word, $index;
                    		}   
                	}
            	}
            	elsif ($index_count > $read_from and $characters[$index_count - 1] eq "\\") {
                	push @word, $index;   
            	}
            	elsif ($index_count > $read_from and $index ne ";" and $index ne "\"" and $index 
            	ne "\'" and $index ne "\n" and $index ne "\\") {
                	push @word, $index;
            	} 
            	$index_count++;
        }
    	$to_print = join("", @word);
    	$line = "print \"$to_print\\n\";";
    	@word = ();
    	@characters = ();   
    	return $line;
}    
# simply edits out some bracket and words to make it work in perl.
sub while_statement {
	$to_replace = $_[0];
	$replacement = $to_replace;
        $replacement =~ s/ test //;
        $replacement =~ s/while/while \(/;
        $replacement =~ s/\n/\) \{/;
        $replacement =~ s/\[//;
        $replacement =~ s/\]/ /;
    return $replacement;
}
# gets rid of expr and the brackets
sub expre {
    	$replacement = $_[0];
    	$replacement =~ s/expr //;
    	$replacement =~ s/\`//g;
    	@word = ();
    	@split = ();

    	return $replacement;        
}
#Finds comments and puts them into a comment array for later use.
sub hash_tag {
    	$replacement = $_[0];
    	$replacement =~ s/^\s+//;
    	@split = split ("", $replacement);
    	$comment_index = index($replacement, "#");
    	$index_count = 0;
    	foreach $character (@split) {
        	if ($index_count < $comment_index) {
            		push @no_comment, $character;
        	}
        	elsif ($index_count >= $comment_index) {
            		push @comment, $character;
        	}
        	$index_count++;         
    	}
    	$replacement = join("", @no_comment);
    	$line_comment = join("", @comment);
    	push @comments, $line_comment;
    	@comment = ();
    	@split = ();
    	@no_comment = ();
    	$replacement =~ s/\s+$//;
    	return $replacement;    
}

    
    
