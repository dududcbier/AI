% isnumber(X) is true if X is a isnumber

isnumber(0).
isnumber(s(X)) :- isnumber(X).

% plus(X,Y,Z) is true if X + Y = Z

plus(0,X,X) :- isnumber(X).
plus(s(X),Y,s(Z)) :- plus(X,Y,Z).

% minus(X,Y,Z) is true if X - Y =Z

minus(X,0,X) :- isnumber(X).
minus(s(X),s(Y),Z) :- minus(X,Y,Z).

% times(X,Y,Z) is true if X * Y = Z

times(X,0,0) :- isnumber(X).
times(X,s(Y),Z) :- times(X,Y,Z1), plus(X,Z1,Z).

% pow(X,Y,Z) is true if X^Y = Z

pow(X,0,s(0)) :- isnumber(X).
pow(X,s(Y),Z) :- pow(X,Y,Z1), times(X,Z1,Z).

% Example queries:
% Isnumbers are represented as successors of 0. For example, 2 is s(s(0)).
% 2+2=4 is plus(s(s(0)), s(s(0)), s(s(s(s(0)))))
% 3*2=? is times(s(s(s(0))), s(s(0)), X)

% even(x) is true if x is an even number
even(0).
even(s(X)) :- odd(X).

% even(x) is true if x is an odd number
odd(0) :- false.
odd(s(X)) :- even(X).

% div2(N,D) is true if D = N / 2
div2(0,0).
div2(s(0), 0).
div2(s(s(N)),s(D)) :- div2(N, D).

% divi2(N,D) is true if D = N / 2
divi2(N,D) :- times(D,s(s(0)),N).
divi2(s(N),D) :- times(D,s(s(0)),N).

% log(X,B,N) is true if B^N = X
log(X,B,N) :- pow(B,N,X).

% fib(X,Y) is true if fib(X) = Y
fib(0,0).
fib(s(0),s(0)).
fib(s(s(X)),Y) :- fib(s(X),F1), fib(X,F2), plus(F1,F2,Y).

% power(X,N,Y) is true if X^N = Y
power(X,0,s(0)).
power(X,s(s(0)),Y) :- times(X, X, Y).
power(X,N,Y) :- even(N), div2(N, D1), power(X,s(s(0)),P1), power(P1,D1,Y).
power(X,s(N),Y) :- odd(s(N)), power(X,N,P1), times(X,P1,Y).