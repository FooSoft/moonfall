function OnWorldCreate(parameters)
    local spawnId = nil
    if parameters.spawn == nil then
        spawnId = ActorFromName("spawnDefault")
    else
        spawnId = ActorFromName(parameters.spawn)
    end

    if spawnId ~= nil then
        playerId = ActorScriptSendMessage(spawnId, "spawn").playerId
        if playerId ~= nil then
            CameraSetTargetActor(playerId)
        end
    end
end
