scriptId = 'com.misahn.Test'

listenForSwipeDownIn = false
listenForSwipeUpOut = false
reset = false

totalDeltaY = 0
lastPitch = 0
minSwipeSpd = 0.05
swipeDistance = 0.15

function onSwipeDown()
	myo.debug("SWIPE DOWN")
	myo.keyboard("down_arrow", "press")
end

function onSwipeUp()
	myo.debug("SWIPE UP")
	myo.keyboard("up_arrow", "press")
end

function onPoseEdge(pose, edge)
	if edge == "on" then
		if pose == "fist" then
			myo.debug("Myo data: Roll " .. myo.getRoll() .. " Yaw " .. myo.getYaw() .. " Pitch " .. myo.getPitch())
			myo.vibrate("short")
		elseif pose == "waveOut" then
			myo.debug("Swipe down started")
			listenForSwipeDownIn = true
		elseif pose == "waveIn" then
			myo.debug("Swipe up started")
			listenForSwipeUpOut = true
		end
	end
end

function onPeriodic()
	-- Get the difference in pitch, which is the vertical angular movement
	local currentPitch = myo.getPitch()
	local deltaY = currentPitch - lastPitch
	lastPitch = currentPitch

	-- We want the difference in pitch to be downwards, and be fast enough
	if deltaY < 0  and math.abs(deltaY) > minSwipeSpd then
		totalDeltaY = totalDeltaY - deltaY
		myo.debug("swiping down")
		-- Check if we've achieved the total downwards movement to count as a swipe
		-- We don't want multiple swipe events to trigger in a short period of time
		-- Note that to trigger this, we need a thumbToPinkyEvent to have happened beforehand
		if listenForSwipeDownIn and math.abs(totalDeltaY) > swipeDistance then
			myo.vibrate("short")
			onSwipeDown()
			totalDeltaY = 0
			reset = true
		end
	elseif math.abs(deltaY) > minSwipeSpd then
		totalDeltaY = totalDeltaY - deltaY
		myo.debug("swiping up")
		-- Check if we've achieved the total downwards movement to count as a swipe
		-- We don't want multiple swipe events to trigger in a short period of time
		-- Note that to trigger this, we need a thumbToPinkyEvent to have happened beforehand
		if listenForSwipeUpOut and math.abs(totalDeltaY) > swipeDistance then
			myo.vibrate("short")
			onSwipeUp()
			totalDeltaY = 0
			reset = true
		end
	else
		-- Reset our distance counter
		totalDeltaY = 0
		if reset then
			reset=false
			listenForSwipeUpOut = false
			listenForSwipeDownIn = false
		end
	end
end

function onForegroundWindowChange(app, title)
	return true
end