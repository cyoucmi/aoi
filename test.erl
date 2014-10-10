-module(test).

-compile(export_all).

test()->
    Space = aoi:create(),
    aoi:update(Space, 1, 'wm', {1.0, 2.0}),
    aoi:update(Space, 2, 'wm', {3.0, 4.0}),
    aoi:message(Space).
