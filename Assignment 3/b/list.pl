len([],0).
len([H|T],N) :- len(T,N1), N is N1+1.

member(X,[]) :- false.
member(X,[H|T]) :- X = H; member(X,T).

concat([HL|TL],[HX|TX],Y) :- HL = HX, concat(TL,TX,Y).
concat([HL|TL],[],[HY|TY]) :- HL = HY, concat(TL,[],TY).
concat([],[],[]).

reverse(L,R) :- reverse(L,[],R).
reverse([],R,R).
reverse([H|T],L,R) :- concat(L2,[H],L), reverse(T,L2,R).

palindrome(L) :- reverse(L,L).