# Infix-Evaluator

This program is an Infix Expression Evaluator using Stack instead of Binary Search Tree

**ASSUMPTIONS**
-	A plus sign is not needed to specify a positive number.
- In order for a negative number to be recognized, the user has to enter the number right after the minus sign.
- Multiple unary operators next to each other are not supported by the program and an error will be thrown.
- For each expression in the input file, if there are errors (syntax/logic errors) in the expression, the program will only throw one error that is encountered first and move on to the next expression.
- The program only supports round brackets “(”, “)”. Any bracket that is different than round bracket will be considered an unknown character.
- All input operands must be integers and the result of a comparison is displayed either 0 (false) or 1 (true)

**UML CLASS DIAGRAM**
<a href="http://imgur.com/Br1VBZa"><img src="http://i.imgur.com/Br1VBZa.png" title="source: imgur.com" /></a>
