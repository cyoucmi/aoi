-module(test).

-compile(export_all).

test()->
    Space = aoi:create(),
    aoi:update(Space, 1, 'wm', 0, 0),
    aoi:update(Space, 2, 'wm', 0, 0),
    aoi:message(Space).
