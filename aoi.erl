-module(aoi).
-export([create/0,update/6,update/5,message/1]).

create()->
    aoi_nif:create().

update(Space,Id,Mode,X,Y,Z)when is_integer(Id), is_atom(Mode), is_number(X), is_number(Y), is_number(Z)->
    aoi_nif:update(Space,Id,Mode,X,Y,Z).

update(Space,Id,Mode,X,Y)->
    aoi_nif:update(Space,Id,Mode,X,Y,0).

message(Space)->
    aoi_nif:message(Space).


