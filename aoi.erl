-module(aoi).
-export([create/0,update/4,message/1]).

create()->
    aoi_nif:create().

update(Space,Id,Mode,{X,Y,Z})when is_integer(Id), is_atom(Mode), is_number(X), is_number(Y), is_number(Z)->
    aoi_nif:update(Space,Id,Mode,float(X),float(Y),float(Z));

update(Space,Id,Mode,{X,Y})->
    aoi_nif:update(Space,Id,Mode,float(X),float(Y),0.0).

message(Space)->
    aoi_nif:message(Space).


