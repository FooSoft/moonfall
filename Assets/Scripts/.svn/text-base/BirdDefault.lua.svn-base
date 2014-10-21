DIRECTION_NORTH = 1
DIRECTION_EAST = 2
DIRECTION_SOUTH = 3
DIRECTION_WEST = 4

function DoActorFly(direction)
    ActorPhysicsSetVelocity(actorId, velocities[direction])
    ActorAnimationSet(actorId, animations[direction])
    ActorAnimationPlay(actorId, true)
end

function OnActorCreate(id, parameters)
    actorId = id
    speed = parameters.speed
    alias = ActorGetAlias(actorId)

    velocities =
    {
        [DIRECTION_NORTH] = {x = 0, y = -speed},
        [DIRECTION_EAST] = {x = speed, y = 0},
        [DIRECTION_SOUTH] = {x = 0, y = speed},
        [DIRECTION_WEST] = {x = -speed, y = 0}
    }

    animations =
    {
        [DIRECTION_NORTH] = alias..".Fly.N",
        [DIRECTION_EAST] = alias..".Fly.E",
        [DIRECTION_SOUTH] = alias..".Fly.S",
        [DIRECTION_WEST] = alias..".Fly.W"
    }

    local directionValues =
    {
        ["north"] = DIRECTION_NORTH,
        ["east"] = DIRECTION_EAST,
        ["south"] = DIRECTION_SOUTH,
        ["west"] = DIRECTION_WEST
    }

    direction = directionValues[parameters.direction]
    DoActorFly(direction)
end

function OnActorUpdate(elapsed)
    local actorPosition = ActorGetPosition(actorId)
    local actorPadding = 512
    local cameraBounds = CameraGetBounds()
    local outsideCamera =
        actorPosition.x < cameraBounds.x0 - actorPadding or actorPosition.y < cameraBounds.y0 - actorPadding or
        actorPosition.x > cameraBounds.x1 + actorPadding or actorPosition.y > cameraBounds.y1 + actorPadding

    if outsideCamera then
        ActorRemove(actorId)
    end
end
