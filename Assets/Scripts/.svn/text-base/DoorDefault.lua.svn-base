DOOR_STATE_OPEN = 1
DOOR_STATE_CLOSED = 2
DOOR_STATE_OPENING = 3
DOOR_STATE_CLOSING = 4

function OnActorCreate(id, parameters)
    doorId = id
    local doorAlias = ActorGetAlias(doorId)
    doorAnimationOpen = doorAlias .. ".Open"
    doorAnimationClose = doorAlias .. ".Close"
    doorWorld = parameters.world
    doorSpawn = parameters.spawn
    doorMaxOpenTime = parameters.openTime
    doorState = DOOR_STATE_CLOSED
    doorOpenTime = 0
end

function OnActorUpdate(elapsed)
    local doorIds = nil

    if doorState == DOOR_STATE_CLOSED or doorState == DOOR_STATE_CLOSING then
        doorIds = ActorPhysicsQueryShapeCollisions(doorId, ACTOR_SHAPE_TYPE_COLLISION_SOLID)
    elseif doorState == DOOR_STATE_OPEN or doorState == DOOR_STATE_OPENING then
        doorIds = ActorPhysicsQueryShapeCollisions(doorId, ACTOR_SHAPE_TYPE_COLLISION_INTERACT)
    end

    assert(doorIds ~= nil)

    for index, value in ipairs(doorIds) do
        if ActorGetName(value) == "player" then
            if doorState == DOOR_STATE_CLOSED or doorState == DOOR_STATE_CLOSING then
                OpenDoor()
            elseif doorState == DOOR_STATE_OPEN or doorState == DOOR_STATE_OPENING then
                WorldLoad(doorWorld, { spawn = doorSpawn })
            end
        end
    end

    if not ActorAnimationIsPlaying(doorId) then
        if doorState == DOOR_STATE_OPENING then
            doorState = DOOR_STATE_OPEN
        elseif doorState == DOOR_STATE_CLOSING then
            doorState = DOOR_STATE_CLOSED
        end
    end

    if doorState == DOOR_STATE_OPEN then
        doorOpenTime = doorOpenTime + elapsed
        if doorOpenTime > doorMaxOpenTime then
            CloseDoor()
        end
    end
end

function OpenDoor()
    ActorAnimationSet(doorId, doorAnimationOpen)
    ActorAnimationPlay(doorId, false)
    doorState = DOOR_STATE_OPENING
    doorOpenTime = 0
end

function CloseDoor()
    ActorAnimationSet(doorId, doorAnimationClose)
    ActorAnimationPlay(doorId, false)
    doorState = DOOR_STATE_CLOSING
end
