
#ifndef _MENU_H_
#define _MENU_H_


char *items[][40] = {{"Basics", "Expression", "Complex", "Polynomial", "Linear Algebra", "Calculus", 
					"Graphics", "Exponential", "Circular", "Hyperbolic", "Logical", "Sentence", 
					"Special", "Misc", "\0"},
					
					{"abs", "mod", "sqrt", "factorial", "lcm", "gcd", "numerator", "denominator", 
					"ceiling", "floor", "\0"},
					{"eval", "simplify", "subst", "factor", "sum", "product", "choose", "rationalize", "inner", "\0"},
					{"arg", "conj", "imag", "mag", "polar", "real", "rect", "clock", "\0"},
					{"coeff", "deg", "expand", "factor", "leading", "nroots", "quotient", "roots", "\0"},
					{"adj", "cofactor", "contract", "det", "dot", "inv", "outer", "transpose", "unit", 
					"zero","simult", "\0"},
					{"defint", "derivative", "gradient", "integral", "taylor", "\0"},
					{"draw", "trange=", "xrange=", "yrange=", "\0"},
					{"circexp", "exp", "expcos", "expsin", "log", "\0"},
					{"arccos", "arcsin", "arctan", "cos", "sin", "tan", "\0"},
					{"arccosh", "arcsinh", "arctanh", "cosh", "sinh", "tanh", "\0"},
					{"and", "or", "not", "test", "testeq", "isprime", "\0"},
					{"for", "do", "check", "stop", "\0"},
					{"besselj", "hermite", "laguerre", "legendre", "\0"},					
					{"eigen", "eigenval", "eigenvec", "binomial", "divisors", "condense", "dirac",
					"sgn", "dim", "rank", "cross", "curl", "filter", "erf", "erfc", "quote", 
					"autoexpand=", "trace=", "\0"}};
#endif	