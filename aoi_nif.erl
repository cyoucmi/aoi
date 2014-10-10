-module(aoi_nif).
-export([create/0,update/6,message/1]).

-on_load(load/0).

load()->
	erlang:load_nif("./aoi_nif", 0).

create()->
	"aoi_nif library not loaded".

update(_,_,_,_,_,_)->
	"aoi_nif library not loaded".

message(_)->
	"aoi_nif library not loaded".


