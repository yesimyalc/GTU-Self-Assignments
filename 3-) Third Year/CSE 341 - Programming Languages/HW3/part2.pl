%facts

flight(canakkale,erzincan,6).

flight(erzincan,canakkale,6).
flight(erzincan,antalya,3).

flight(antalya,erzincan,3).
flight(antalya,izmir,2).
flight(antalya,diyarbakýr,4).

flight(izmir,antalya,2).
flight(izmir,istanbul,2).
flight(izmir,ankara,6).

flight(istanbul,izmir,2).
flight(istanbul,ankara,1).
flight(istanbul,rize,4).

flight(rize,istanbul,4).
flight(rize,ankara,5).

flight(ankara,rize,5).
flight(ankara, istanbul,1).
flight(ankara,izmir,6).
flight(ankara,van,4).
flight(ankara,diyarbakýr,8).

flight(diyarbakýr,antalya,4).
flight(diyarbakýr,ankara,8).

flight(van,ankara,4).
flight(van,gaziantep,3).

flight(gaziantep,van,3).

%rules

sum([], 0).
sum([H|T], N):-
    sum(T, X),
    N is X + H.

route(X,Y,C) :- routex(X,Y,C,[],[]).
routex(X,Y,C,VisitList,DistanceList) :- flight(X,Z,M), not(member(Z, VisitList))
						, (Y = Z, sum([M|DistanceList], C); routex(Z, Y, C, [X | VisitList],[M|DistanceList])).






