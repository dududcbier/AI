% predicate north(x,y) is true when x is north of y
north(m,i).
north(j,f).
north(f,b).
north(k,g).
north(g,c).
north(p,l).
north(l,h).
north(h,d).

% predicate south(x,y) is true when x is south of y
south(X,Y) :- north(Y,X).

% predicate west(x,y) is true when x is west of y
west(n,o).
west(m,n).
west(j,k).
west(e,f).
west(a,b).
west(c,d).

% predicate east(x,y) is true when x is east of y
east(X,Y) :- west(Y,X).

% List operations
member(X,[]) :- false.
member(X,[H|T]) :- X = H; member(X,T).

concat([HL|TL],[HX|TX],Y) :- HL = HX, concat(TL,TX,Y).
concat([HL|TL],[],[HY|TY]) :- HL = HY, concat(TL,[],TY).
concat([],[],[]).

% path finding
path(X,X,L).
path(X,Y) :- path(X,Y,[]).
path(X,Y,Path) :- east(E,X),  not(member(E,Path)), concat(NewPath,[E],Path), path(E,Y,NewPath); 
			 	  north(N,X), not(member(N,Path)), concat(NewPath,[N],Path), path(N,Y,NewPath); 
			 	  south(S,X), not(member(S,Path)), concat(NewPath,[S],Path), path(S,Y,NewPath); 
			 	  west(W,X),  not(member(W,Path)), concat(NewPath,[W],Path), path(W,Y,NewPath). 




