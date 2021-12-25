(defvar *Keywords* (list "and" "or" "not" "equal" "less" "nil" "list" "append" "concat" "set"
                    "deffun" "for" "if" "exit" "load" "disp" "true" "false"))

(defvar *KWs* (list "KW_AND" "KW_OR" "KW_NOT" "KW_EQUAL" "KW_LESS" "KW_NIL" "KW_LIST" "KW_APPEND" "KW_CONCAT" "KW_SET"
               "KW_DEFFUN" "KW_FOR" "KW_IF" "KW_EXIT" "KW_LOAD" "KW_DISP" "KW_TRUE" "KW_FALSE"))

(defvar *Operators* (list "+" "-" "/" "*" "**" "(" ")" "\"" "\"" ","))
(defvar *OPs* (list "OP_PLUS" "OP_MINUS" "OP_DIV" "OP_MULT" "OP_DBLMULT" "OP_OP" "OP_CP" "OP_OC" "OP_CC" "OP_COMMA"))

(defvar *op_oc_count* 0)

(defvar *tokens* (list))
(defvar *filename*)

(defun find_format ()
	(format t "If you wanna read from a file enter filename. Else enter start. ~%")
    (format t "g++ ")
    (let ((rline) (format_result))
        (setq rline (read-line))
        (terpri)
        (if (string= rline "start")
            (setq format_result 0)
            (progn
                (setq format_result 1)
                (setq *filename* rline)
             )
        )
        (return-from find_format format_result)
    )
)

;;Takes a line of strings as parameter. Separates the strings in the line and stores in word_list
;;The loop runs till the index=length of the line
;;In the loop condition checks each character if it is a space or tab. If it is not the character is concatenated to word
;;If a space or tab is found word is stored in word_list and then emptied.
;;When the loop ends there might still be a word left in the word. If word is not null it is added to word_list
;;word_list is reversed and returned
(defun split_line (line)
  (let ((word nil) (index 0) (ch) (word_list (list)))
  	(loop
  		(setf ch (string (char line index)))
		(cond
			((or (string= ch #\Space) (string= ch #\Tab) (string= ch #\return) (string= ch #\linefeed) (string= ch ""))
                (if (not (eql word nil))
                    (progn
                        (setf word_list (cons word word_list))
                        (setf word nil)
                    )
                )
            )
			( t
                (setf word ( concatenate 'string word ch )))
		)
		(setf index (+ index 1))
		(when (= (length line) index)
            (return word_list))
	)
	(if (not (eql word nil))
        (setf word_list (cons word word_list)) )
	(reverse word_list)
  )
)

(defun isOP(op)
    (let ((index 0))
        (cond
            ((and (string= op "\"") (= 0 *op_oc_count*))
                (setf *op_oc_count* 1)
                (return-from isOP 7)
            )
            ((and (string= op "\"") (= 1 *op_oc_count*))
                (setf *op_oc_count* 0)
                (return-from isOP 8)
            )
            (t
                (loop for temp in *Operators*
                    do
                    (if (string= temp op)
                        (let()
                            (return-from isOP index)))
                    (setf index (+ 1 index))
                )
                -1
            )
        )
    )
 )

 (defun isValue(value)
	(let ((index 0) (ch) (dot_c 0) (temp_c_c))
	    (setf temp_c_c *op_oc_count*)
	    (setf ch (char value index))
	    (when (and (/= 1 (length value)) (string= ch "0"))
	        (setf ch (char value 1))
	        (cond
	            ((/= (isOP (string (char value 1))) -1)
	                (setf *op_oc_count* temp_c_c)
	                (return-from isValue 1))
	            ((> (length value) 2)
	                (setf index 2)
	            )
	            ((and (= (isOP (string (char value 1))) -1) (string/= ch "."))
	                (setf *op_oc_count* temp_c_c)
	                (return-from isValue 0))
	        )
	    )
		(loop
			do
			(setf ch (char value index))
			(cond
			    ((and (not(digit-char-p ch)) (= dot_c 0) (string= ch "."))
			        (setf dot_c 1))
			    ((and (not(digit-char-p ch)) (= dot_c 1) (string= ch "."))
			        (return-from isValue 0))
			    ((and (not(digit-char-p ch)) (= (isOP (string (char value index))) -1))
			        (return-from isValue 0))
			)
			(setq index (+ 1 index))

			(when (or (= index (length value)) (/= (isOP (string (char value index))) -1))
			    (setf dot_c 0)
			    (setf *op_oc_count* temp_c_c)
			    (return index))
		)
	    index
  	)
)

(defun isID(ID)
	(let ((index 0) (ch) (temp_c_c))
	    (setf temp_c_c *op_oc_count*)
	    (setf ch (char ID index))
	    (when (digit-char-p ch)
	        (return-from isID 0))
		(loop
			do
			(setf ch (char ID index))
			(if (not (or (alpha-char-p  ch) (digit-char-p ch) (char= #\_ ch) ))
                (return-from isID 0))
			(setq index (+ 1 index))

			(when (or (= index (length ID)) (/= (isOP (string (char ID index))) -1))
			    (setf *op_oc_count* temp_c_c)
			    (return index))
		)
	index
  	)
)

(defun isKeyWord(keyword)
	(let ((index 0) (ch) (word nil)(err 0) (kIndex 0) (temp_c_c))
	    (setf temp_c_c *op_oc_count*)
		(loop
			do
			(setf ch (char keyword index))
			(if (not(alpha-char-p  ch)) (setf err 1))
			(setf word ( concatenate 'string word (string ch)))
			(setq index (+ 1 index))
			(when (or (= index (length keyword)) (/= (isOP (string (char keyword index))) -1))
			    (setf *op_oc_count* temp_c_c)
			    (return index) )
		)
		(if (= err 1) (return-from isKeyWord (list -1 index)))
		(loop for temp in *Keywords*
	  		do
	  			(if (string= temp word)
	  				(return kIndex)
	  				(setf kIndex (+ 1 kIndex))
		  		)
		)
		(if (= kIndex (length *Keywords*)) (list -1 index) (list kIndex index))
  	)
)

(defun analyze_word (word ostream)
    (let ((subword nil) (index 0) (ch) (size) (lIndex) (rIndex) (opIndex 0))
    (setf size (length word))
        (loop while (/= index size)
            do
            (setf ch (string (char word index)))
            (setf subword (subseq word index))
            (cond
                ((/= (setq rIndex (isOP ch)) -1 )
                    (let ()
						(setf index (+ 1 index))
                        (setf opIndex index)
                        (if (string= (nth rIndex *Operators*) "*")
                            (progn
                                (if (= size index)
                                    (progn
                                        (print (nth rIndex *OPs*))
                                        (return 1)))
                                (if (string= (string (char word index)) "*")
                                    (let ()
										(setf rIndex (+ 1 rIndex))
										(setf index (+ 1 index))
                                        (setf opIndex index)
									)
                                 )
                             )
                        )
                        (print (nth rIndex *OPs*))
                        (format ostream "~A~c" (nth rIndex *OPs*) #\NewLine)

                    )
                 )
                 ((/= (setq rIndex (isValue subword)) 0)
					(let ()
						(print "VALUE")
                        (format ostream "\"VALUE\"~c" #\NewLine)
                        (setf index (+ index rIndex))
					)
				)
				((/= -1 (car (setq lIndex (isKeyWord subword))))
					(let ()
						(print (nth (car lIndex) *KWs*))
                        (format ostream "~A~c" (nth (car lIndex) *KWs*) #\NewLine)
						(setf index (+ index (cadr lIndex)))
					)

				)
				((/= (setq rIndex (isID subword)) 0)
					(let ()
						(print "IDENTIFIER")
                        (format ostream "\"IDENTIFIER\"~c" #\NewLine)
                        (setf index (+ index rIndex))
					)
				)
                ( t
					(let (tempWord)
						(setf tempWord (subseq word opIndex size))
						(format t "~%SYNTAX_ERROR: ~A cannot be tokenized~c" tempWord #\NewLine)
                        (format ostream "~%SYNTAX_ERROR ~A cannot be tokenized~c" tempWord #\NewLine)
                        (return-from analyze_word -1)
					)
				)
            )
         )
     )
    1
)

;;Takes a line to analyze the words
;;Checks if the line is a comment first. If it is, prints COMMENT then directly returns 1 meaning line is analyzed properly.
;;If it is not a comments, the line is separated into words and stored in word_list
;;For each word in the word_list analyze_word function is called to tokenize it and then print the token
;;If the word can be tokenized returns 1. Returns 0 for exit and returns -1 for UNKNWON
(defun analyze_line (line ostream)
	(let ((word_list nil)(check 0))
        (when (and (> (length line) 1) (char= (char line 0) #\;) (char= (char line 1) #\;))
            (progn
                (format t "~%\"COMMENT\"")
                (format ostream "~%\"COMMENT\"")
                (return-from analyze_line 1)
            )
         )
        (when (< (length line) 1)
            (return-from analyze_line 1))
		(setf word_list (split_line line))
		(loop for temp in word_list
	  		do
            (setq check(analyze_word temp ostream))
            (when (= check -1)
                (return-from analyze_line -1))
        )
    )
   1
)

;;gppinterpreter is a function which takes 0 or 1 parameters.
;;parameter must be a file name
;;if no parameter is given, input from user is taken and the input is evaluated line by line
;;/if filename is given as parameter, input is taken from a file and it is evaluated line by line till the end of the file.
(defun gppinterpreter (&optional (filename -1))
	(if (equal filename -1)
		(let ((line) (line_check))
            (with-open-file (ostream "output.txt" :direction :output :if-exists :supersede :if-does-not-exist :create)
                (loop
                   (format t ">")
                   (setq line (read-line))
                   (setq line_check (analyze_line line ostream))
                   (terpri)
                   (when (= line_check -1)
                        (print "Lexer closing due to an error.")
                        (return))
                )
             )
		)
		(let ((input_file (open filename :if-does-not-exist nil)))
            (with-open-file (ostream "output.txt" :direction :output :if-exists :supersede :if-does-not-exist :create)
                (when input_file
                    (loop for line = (read-line input_file nil)
                        while line do
                            (analyze_line line ostream)
                            (terpri)
                            (format ostream "~%"))
                    (close input_file)
                    (print "End of file. Lexer is closing.")
                )
             )
		)
	)
)

(defvar *format_input* (find_format))
(if (= *format_input* 0)
    (gppinterpreter))
(if (= *format_input* 1)
    (gppinterpreter *filename*))




