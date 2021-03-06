# compiler-construction-tasks

Adding new features to an existing compiler.

## Task 1: Emulation of shift logical left/right
Added constants for sll and srl f3 and f7 codes, according to documentation.
In decode_execute() there is already a distinction between OP codes and the decoding of their instruction typs.
	Accoding to docu, sll and srl Op code is 51(in selfie OP_OP), like ADD, SUB,..
	Added distinction (if-clauses) for sll (F3_SLL = 1 && F7_SLL = 0) and srl (F3_SRL = 5 && F7_SRL = 0)
	We now know when we are dealing with a sll or srl operation - now we start do_sll()/do_srl() to tigger their semantics/effects on the machine.
	Copied and adjusted debug code from other instruction.
In do_sll()/do_srl()
	According to documentation, all sll/srl operations read the rs1 and rs2 registers as source operands and write the result into register rd.
	So the semantics/code for do_sll()/do_srl() is the same as do_add(), only instead of a addition between rs1 and rs2, we use the selfie library functions to leftshift/rightshift(the bitsequence rs1, by rs2 bits).
	Of course we also have to increment the program counter.
Added debug constants ic_sll/ic_srl, their semantics and output.

## Task 2 Parsing of << and >>
Added constants for sll and srl symbols.
In scanner (getSymbol())
	After the Check for Less/Greater signs we have to differentiate if we read << or >> (like it is done for <= and >=) and set the matching symbol.
In Parser
	According to c standart the precedence of << and >> is in between our grammars simple expression and expression
	First, we copy the simpleExpression function (complexExpression) and "attach it" in between the other two
		We need to change the compile calls from expression->simpleExpression to expression->complexExpression->simpleExpression
	In complexExpression
		First we take our left hand simpleExpression and check for an occurence of << or >>
		If we find it, we also take the right hand simpleExpression and emit the corresponding machinecode (R-Type, like SLTU, but with different funct codes from hw1/the riscV doku)
		The result is left as usual in the first temporary register, while the second one gets emitted.
		For parsing a complexExpression, we need a while loop because we want to be able to parse chained complexExpressions f.e. a << b << c

## Multi dimensional arrays
Build linkedlist[next,payload] infrastructure for saving the arrays dimensions. Make setter, getter, addDimension and getArraySize.
In compile_cstar()
	Parse arraybrackets without error.
	During parsing retrieve literal for every bracket/dimension and add it to dimensionsList
	Allocate memory for array in data segment, depending on its size (getArraySize() multiplies all dimensions of the dimensionList)
In emitGlobalsStrings()
	Also increase binaryLength depending on getArraySize()
In compile_factor()
	If factor is array don't just getAddress() but also compute and add the offset
In computeArrayOffset()
	Now we need every single dimensions (maximum) size, which has been saved in our linkedlist in the symboltableentry of the array.
	We allocate a register for the sum.
	While we parse the brackets we load the inner expressions (so we support more than just literals inside brackets for assignments and access).
	For every read dimensions value, we calculate its "weight". The weight of a dimension is the multiplication of all following dimensions (maximum) sizes.
	We sum up every dimensions value multiplied by its weight (the last dimensions weight is always 1).
	Finally we have the offset in our currentTemporary().
In compile_statement()
	If left hand expression of '=' is an array we computeArrayOffset() and add the arrays baseaddress (getAddress(arrayEntry)).
	After the right hand expression is evaluated and in a new register, we store the expression (currentTemporary()) to our computed array address (previousTemporary())
Problems
	Everything was working fine in my testfile and selfie after adding array registers. After adding array power of two table, i had errors in selfie.
	It turned out that the arrays address was greater than the maximum immediate value for ADDI. Using load_integer() and ADD solves this problem.

## Structs
Created a new SYMBOL "SYM_STRUCT" and new TYPE "STRUCT_T"
In compile_cstar() we start compile_structDef() if we encounter a SYM_STRUCT
In compile_structDef() we build out structs field list and add it to the symboltable
	We parse and save struct type name and type
	We loop until we find no more types (struct fields)
		We save the fields name
		If we find a type struct we have to save its type too
In compile_statement() we start compile_structDeclaration() to parse pointer-to-struct declarations and allocate memory for it
In compile_factor() we start compile_arrayAccess() to compute the correct offset of the field we are trying to access.
	For this we are retrieving the saved fieldslist (from the definition) from of the symboltable

## For loop
Take compile_while() as template for compile_for() and introduce SYM_FOR symbol.
	Add SYM_FOR to keywords- and statement-checks and direct to compile_for().
	Let FOR work exactly like WHILE for starters.
In compile_for()
	For the initialisation statement add parsing and releasing of compile_statement() before the condition statement.
	After the condition statement do the same for the increment statement
	Now we have the following code structure: init - cond - inc - body - afterBody
		To let the increment statement trigger after the loop body and before the condition we need to chain our jumps as follows:
			After condition branch to body or end (in any case skip the increment statement, address fixup needed)
			After body jump to increment
			After increment jump to condition
