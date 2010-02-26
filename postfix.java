/* Simple top-down predictive parser 
 * Grammar (left-recursive): 
 * expr -> expr + term {print('+')} | expr - term {print('-')} | term
 * term -> 0..9{print('0')..print('9')}
 * Transform: 
 * A -> Aa | Ab | c
 * into
 * A -> cR
 * R -> aR | bR | E
 * New grammar (right-recursive) :
 * expr -> term rest
 * rest -> + term {print('+') } rest | - term {print('-')} rest | E
 * term -> 0{print('0')}...9{print('9')}
 */
import java.io.*;
class parser {
	static int lookAhead;

	public parser() throws IOException {
		lookAhead = System.in.read();
	}
	void rest() throws IOException {
		if ( lookAhead == '+' ) {
			match('+'); term(); System.out.write('+'); rest();
		}
		else if ( lookAhead == '-' ) {
			match('-'); term(); System.out.write('-'); rest();
		}
		else return;
	}

	void expr() throws IOException {
		term(); rest();
		/*
		while ( true ) {
			if ( lookAhead == '+' ) {
				match('+'); term(); System.out.write('+');
			}
			else if ( lookAhead == '-' ) {
				match('-'); term(); System.out.write('-');
			}
			else return;
		}
		*/
	}

	void term() throws IOException {
		if ( Character.isDigit((char)lookAhead) ) {
			System.out.write((char)lookAhead); match(lookAhead);
		}
		else throw new Error("Syntax error");
	}

	void match(int t) throws IOException {
		if ( lookAhead == t ) lookAhead = System.in.read();
		else throw new Error("Syntax error");
	}
}

public class postfix {
	public static void main(String[] args) throws IOException {
		parser parse = new parser();
		parse.expr(); System.out.write('\n');
	}
}
