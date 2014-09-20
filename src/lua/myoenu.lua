scriptId = 'com.misahn.Test'

currentPose = nil
listenForSwipe = false

totalDeltaY = 0
lastPitch = 0
minSwipeSpd = 0.07
swipeDistance = 0.2
lastSwipeTime = 0
betweenSwipeDurationMS = 1500

function onSwipeDown()
	myo.debug("SWIPE")
	myo.keyboard("down_arrow", "down")
	myo.keyboard("down_arrow", "up")
end

function onPoseEdge(pose, edge)
	if edge == "on" then
		currentPose = pose
		if pose == "fist" then
			myo.debug("Myo data: Roll " .. myo.getRoll() .. " Yaw " .. myo.getYaw() .. " Pitch " .. myo.getPitch())
			myo.vibrate("short")
		elseif pose == "waveOut" then
			myo.debug("Swipe started")
			listenForSwipe = true
		end
	else
		currentPose = nil
	end
end

function onPeriodic()
	-- Get the difference in pitch, which is the vertical angular movement
	local currentPitch = myo.getPitch()
	local deltaY = currentPitch - lastPitch
	lastPitch = currentPitch

	-- We want the difference in pitch to be downwards, and be fast enough
	if deltaY < 0 and math.abs(deltaY) > minSwipeSpd then
		totalDeltaY = totalDeltaY - deltaY
		myo.debug("swiping down")
		-- Check if we've achieved the total downwards movement to count as a swipe
		-- We don't want multiple swipe events to trigger in a short period of time
		-- Note that to trigger this, we need a thumbToPinkyEvent to have happened beforehand
		local curTime = myo.getTimeMilliseconds()
		if listenForSwipe and math.abs(totalDeltaY) > swipeDistance and (curTime - lastSwipeTime) > betweenSwipeDurationMS then
			myo.vibrate("short")
			onSwipeDown()
			totalDeltaY = 0
			lastSwipeTime = curTime
			listenForSwipe = false
		end
	else
		-- Reset our distance counter
		totalDeltaY = 0
	end
end

function onForegroundWindowChange(app, title)
	return true
end