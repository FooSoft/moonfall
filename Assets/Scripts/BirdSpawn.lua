DIRECTION_NORTH = 1
DIRECTION_EAST = 2
DIRECTION_SOUTH = 3
DIRECTION_WEST = 4

FLOCK_SPREAD = 10
H_SPAWN = 320
V_SPAWN = 240

function OnActorCreate(id, parameters)
    spawnerId = id
    duration = parameters.spawningDuration
    minSpawnTime = parameters.minSpawnTime
    maxSpawnTime = parameters.maxSpawnTime
    minFlockCount = parameters.minFlockCount
    maxFlockCount = parameters.maxFlockCount
    minSpeed = parameters.minSpeed
    maxSpeed = parameters.maxSpeed

    ActorSetShape(
        spawnerId,
        ACTOR_SHAPE_TYPE_COLLISION_SOLID,
        { size = {x = 32, y = 48 }, position = { x = 0, y = 0 }, data = 0, enabled = true}
    )

    directions =
    {
        [DIRECTION_NORTH] = "north",
        [DIRECTION_EAST] = "east",
        [DIRECTION_SOUTH] = "south",
        [DIRECTION_WEST] = "west"
    }

    birdTimer = 0
end

function OnActorUpdate(elapsed)
    if birdTimer <= 0 then

        local camPos = CameraGetTarget()
        local directionValue = math.random(1,4)

        if directionValue == DIRECTION_SOUTH then
            start = {x=math.random(camPos.x - H_SPAWN, camPos.x + H_SPAWN),y=camPos.y - V_SPAWN}
            nxtBird = {{x=-FLOCK_SPREAD,y=-FLOCK_SPREAD}, {x=FLOCK_SPREAD,y=-FLOCK_SPREAD}}

        elseif directionValue == DIRECTION_NORTH then
            start = {x=math.random(camPos.x - H_SPAWN, camPos.x + H_SPAWN),y=camPos.y + V_SPAWN}
            nxtBird = {{x=-FLOCK_SPREAD,y=FLOCK_SPREAD}, {x=FLOCK_SPREAD,y=FLOCK_SPREAD}}

        elseif directionValue == DIRECTION_WEST then
            start = {x=camPos.x + H_SPAWN, y=math.random(camPos.y - V_SPAWN, camPos.y + V_SPAWN)}
            nxtBird = {{x=FLOCK_SPREAD,y=FLOCK_SPREAD}, {x=FLOCK_SPREAD,y=-FLOCK_SPREAD}}

        elseif directionValue == DIRECTION_EAST then
            start = {x=camPos.x - H_SPAWN, y=math.random(camPos.y - V_SPAWN, camPos.y + V_SPAWN)}
            nxtBird = {{x=-FLOCK_SPREAD,y=FLOCK_SPREAD}, {x=-FLOCK_SPREAD,y=-FLOCK_SPREAD}}
        end

        local bird = 0
        local distanceRatio = 0
        local flockCount = math.random(minFlockCount, maxFlockCount)
        local speed = math.random(minSpeed, maxSpeed)
        while bird < flockCount do
            if (math.mod(bird,2) == 0) then
                distanceRatio = bird
            end
            ActorAdd("Bird.01",
                    {x=start.x + (nxtBird[math.mod(bird,2) + 1].x * distanceRatio),
                     y=start.y + (nxtBird[math.mod(bird,2) + 1].y * distanceRatio)},
                     nil,
                     {direction=directions[directionValue], speed=speed})
            bird = bird + 1
        end

        birdTimer = math.random(minSpawnTime, maxSpawnTime)
    end
    birdTimer = birdTimer - elapsed
end
