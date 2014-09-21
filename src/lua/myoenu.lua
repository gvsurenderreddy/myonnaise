scriptId = 'com.misahn.Test'

listenForSwipeDownIn = false
listenForSwipeUpOut = false
reset = false
verticalMove = false
horizontalMove = true

totalDeltaY = 0
lastPitch = 0
totalDeltaX = 0
lastYaw = 0
minSwipeSpd = 0.07
swipeDistance = 0.20
currentPose = "none"

function onSwipeDown()
	myo.debug("SWIPE DOWN")
	myo.keyboard("down_arrow", "press")
end

function onSwipeUp()
	myo.debug("SWIPE UP")
	myo.keyboard("up_arrow", "press")
end

function onSwipeRight()
	myo.debug("SWIPE RIGHT")
	myo.keyboard("right_arrow", "press")
end

function onSwipeLeft()
	myo.debug("SWIPE LEFT")
	myo.keyboard("left_arrow", "press")
end

function onOpenMenu()
	myo.debug("OPEN MENU")
	myo.keyboard("right_shift", "press", "control", "alt", "shift")
end

function onPoseEdge(pose, edge)
	if edge == "on" then
		currentPose = pose
		if pose == "fist" then
			myo.vibrate("short")
			myo.debug("Fist")
			listenForSwipeDownIn = true
			listenForSwipeUpOut = true
		elseif pose == "waveOut" then
			myo.vibrate("short")
			myo.debug("Swipe down/left started")
			listenForSwipeDownIn = true
		elseif pose == "waveIn" then
			myo.vibrate("short")
			myo.debug("Swipe up/right started")
			listenForSwipeUpOut = true
		end
	end
end

function onPeriodic()
	-- Get the difference in pitch, which is the vertical angular movement
	local currentPitch = myo.getPitch()
	local deltaY = currentPitch - lastPitch
	lastPitch = currentPitch
	-- Get the difference in yaw, which is the horizontal angular movement
	local currentYaw = myo.getYaw()
	local deltaX = currentYaw - lastYaw
	lastYaw = currentYaw
	local ThresholdX = (math.abs(deltaX) < minSwipeSpd)
	local ThresholdY = (math.abs(deltaY) < minSwipeSpd)

	-- We want the difference in pitch to be downwards, and be fast enough
	if deltaY < 0  and math.abs(deltaY) > minSwipeSpd and currentPose ~= "fist" then
		totalDeltaY = totalDeltaY - deltaY
		myo.debug("swiping down")
		-- Check if we've achieved the total downwards movement to count as a swipe
		-- We need a thumbToPinkyEvent to have happened beforehand
		if listenForSwipeDownIn and math.abs(totalDeltaY) > swipeDistance then
			myo.vibrate("short")
			onSwipeDown()
			totalDeltaY = 0
			reset = true
			verticalMove = true
		end
	-- We want the difference in pitch to be upwards, and be fast enough
	elseif math.abs(deltaY) > minSwipeSpd and currentPose ~= "fist" then
		totalDeltaY = totalDeltaY - deltaY
		myo.debug("swiping up")
		-- Check if we've achieved the total upwards movement to count as a swipe
		-- We need a thumbToPinkyEvent to have happened beforehand
		if listenForSwipeUpOut and math.abs(totalDeltaY) > swipeDistance then
			myo.vibrate("short")
			onSwipeUp()
			totalDeltaY = 0
			reset = true
			verticalMove = true
		end
	end

	-- We want the difference in yaw to be to the left, and be fast enough
	if deltaX > 0  and math.abs(deltaX) > minSwipeSpd then
		totalDeltaX = totalDeltaX - deltaX
		myo.debug("swiping left")
		-- Check if we've achieved the total movement to the left to count as a swipe
		-- We need a thumbToPinkyEvent to have happened beforehand
		if listenForSwipeDownIn and math.abs(totalDeltaX) > swipeDistance then
			myo.vibrate("short")
			if currentPose == "fist" then
				onOpenMenu()
				horizontalMove = false
				verticalMove = false
				totalDeltaX = 0
				totalDeltaY = 0
				reset=false
				listenForSwipeUpOut = false
				listenForSwipeDownIn = false
			else
				onSwipeLeft()
			end
			totalDeltaX = 0
			reset = true
			horizontalMove = true
		end
	-- We want the difference in pitch to be to the right, and be fast enough
	elseif math.abs(deltaX) > minSwipeSpd then
		totalDeltaX = totalDeltaX - deltaX
		myo.debug("swiping right")
		-- Check if we've achieved the total movement to the right to count as a swipe
		-- We need a thumbToPinkyEvent to have happened beforehand
		if listenForSwipeUpOut and math.abs(totalDeltaX) > swipeDistance then
			myo.vibrate("short")
			if currentPose == "fist" then
				onOpenMenu()
				horizontalMove = false
				verticalMove = false
				totalDeltaX = 0
				totalDeltaY = 0
				reset=false
				listenForSwipeUpOut = false
				listenForSwipeDownIn = false
			else
				onSwipeRight()
			end
			totalDeltaX = 0
			reset = true
			horizontalMove = true
		end
	end
	-- check if the swipe has ended and reset the variables
	if (horizontalMove and ThresholdX) or (verticalMove and ThresholdY) then
		-- Reset our distance counter
		horizontalMove = false
		verticalMove = false
		totalDeltaX = 0
		totalDeltaY = 0
		-- Reset booleans if already performed action
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