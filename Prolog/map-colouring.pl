ngb(albania, [greece, yugoslavia]).
ngb(andorra, [france, spain]).
ngb(austria, [czechoslovakia, hungary, italy, liechtenstein,
              switzerland, westgermany, yugoslavia]).


colours([]).

colours([Country/Colour | Rest]) :-
  colours(Rest),
  member(Colour, [yellow, blue, red, green]),
  not(member(Country1/Colour, Rest), neighbour(Country, Country1)).

neighbour(Country, Country1) :-
  ngb(Country, Neighbours),
  member(Country1, Neighbours).

