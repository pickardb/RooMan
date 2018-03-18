-- This information is used by the Wi-Fi dongle to make a wireless connection to the router in the Lab
SSID = "Adilphone"
SSID_PASSWORD = "adkyd1595"

-- configure ESP as a station
wifi.setmode(wifi.STATION)
wifi.sta.config(SSID,SSID_PASSWORD)
wifi.sta.autoconnect(1)

-- The following 2 pieces of information are related to your Twilio account 
-- see your instructor for missing details
TWILIO_ACCOUNT_SID = "AC388527a0f23ae8d3276c2df9a3456af5"
TWILIO_TOKEN =       "bbd68018afed5b204f9e65bd62418ad3"

-- Unfortunately, the Wi-FI dongle can only make unsecured HTTP requests, but Twilio requires 
-- secured HTTPS requests, so we will use a relay website to convert HTTP requests into HTTPS requests
-- visit http://iot-https-relay.appspot.com/ to learn more about this service
-- Please be sure to understand the security issues of using this relay app and use at your own risk.

-- this is the web address of the relay web site that our dongle sends the initial HTTP request to
HOST = "iot-https-relay.appspot.com" 

-- The following variable defines the TWILIO web site that we will connect to
-- use the first one if you want to send a text to a cell phone
-- use the second (commented out) one if you want to make a call to a cell phone � that�s the only change
URI = "/twilio/Messages.json"
PATH = "/Rooms.json"
HOST_DB = "35.202.73.165"
URI_ROOM = "/Rooms"
URI_USER = "/Users"

--URI = "/twilio/Calls.json"

function build_post_request(host, uri, data_table)

     data = ""

     for param,value in pairs(data_table) do
          data = data .. param.."="..value.."&"
     end

	 data = string.sub(data, 0, -2)
	   
     request = "POST "..uri.." HTTP/1.1\r\n"..
     "Host: "..host.."\r\n"..
     "Connection: close\r\n"..
     "Content-Type: application/x-www-form-urlencoded\r\n"..
     "Content-Length: "..string.len(data).."\r\n"..
     "\r\n"..
     data
     print(request)
     return request
end

function send_sms(body)
	 from = "(514) 613-7185"
	 to = "(604) 396-4772"
     data = {
      sid = TWILIO_ACCOUNT_SID,
      token = TWILIO_TOKEN,
      Body = string.gsub(body," ","+"),
      From = from,
      To = to
     }

     socket = net.createConnection(net.TCP,0)
     socket:on("receive",display)
     socket:connect(80,HOST)

     socket:on("connection",function(sck)
          post_request = build_post_request(HOST,URI,data)
          sck:send(post_request)
     end)
end

function build_database_request(host, uri, data_table, path)

     data = ""

     for param,value in pairs(data_table) do
          data = data .. param.."="..value.."&"
     end

	 data = string.sub(data, 0, -2)
     data = data_table['data']
	   
     request = "PATCH "..uri..path.." HTTP/1.1\r\n"..
     "Host: "..host.."\r\n"..
     "Connection: close\r\n"..
     "Content-Type: text/html\r\n"..
     "Content-Length: "..string.len(data).."\r\n"..
     "\r\n"..
     data
     print(request)
     return request
end

function get_database_request(host, uri, data_table, path)

     data = ""

     for param,value in pairs(data_table) do
          data = data .. param.."="..value.."&"
     end

	 data = string.sub(data, 0, -2)
     data = data_table['data']
	   
     request = "GET "..uri..path.." HTTP/1.1\r\n"..
     "Host: "..host.."\r\n"..
     "Connection: close\r\n"..
     "Content-Type: text/html\r\n"..
     "Content-Length: "..string.len(data).."\r\n"..
     "\r\n"..
     data
     print(request)
     return request
end

-- This function registers a function to echo back any response from the server, to our DE1/NIOS system 
-- or hyper-terminal (depending on what the dongle is connected to)
function display(sck,response)
     print(response)
end

-- When using send_sms: the "from" number HAS to be your twilio number.
-- If you have a free twilio account the "to" number HAS to be your twilio verified number.


function send_rooms_db(data_table)

	json = "{\"inUse\" : "..data_table["use"]..",".."\"lightsOn\" : "..data_table["light"]..",".."\"locked\" : "..data_table["lock"]..",".."\"occupied\" : "..data_table["occup"]..",".."\"temperature\" : "..data_table["temp"].."}"	
		--	"\"roomNum\" : "..data_table["roomNum"]..", "..
		--	"\"occupied\" : "..data_table["occupancy"]..", "..
		--	"\"temperature\" : "..data_table["temp"].."}"
		
	
	data = {
	--path = "/DefaultRoomId",
    data = json
	}
	
	--path = "/DefaultRoomId.json"
	
	socket = net.createConnection(net.TCP,0)
	socket:on("receive", display)
    socket:connect(80,HOST_DB)
	
	socket:on("connection", function(sck)
	post_request = build_database_request(HOST_DB, URI_ROOM, data, PATH)
	sck:send(post_request)
    end)
end


function get_rooms_db(data_table)

	json = "{\"inUse\" : "..data_table["use"]..",".."\"lightsOn\" : "..data_table["light"]..",".."\"locked\" : "..data_table["lock"]..",".."\"occupied\" : "..data_table["occup"]..",".."\"temperature\" : "..data_table["temp"].."}"	
		--	"\"roomNum\" : "..data_table["roomNum"]..", "..
		--	"\"occupied\" : "..data_table["occupancy"]..", "..
		--	"\"temperature\" : "..data_table["temp"].."}"
		
	
	data = {
	--path = "/DefaultRoomId",
    data = json
	}
	
	--path = "/DefaultRoomId.json"
	
	socket = net.createConnection(net.TCP,0)
	socket:on("receive", display)
    socket:connect(80,HOST_DB)
	
	socket:on("connection", function(sck)
	post_request = get_database_request(HOST_DB, URI_ROOM, data, PATH)
	sck:send(post_request)
    end)
end


function patch_rooms_firebase(data_path, use_var, light_var, lock_var, occup_var, temp_var)    
	ip = wifi.sta.getip()

	if(ip==nil)then
		print("No connection established")
	else
		tmr.stop(0)
		json_data = {
			use = use_var,
			light = light_var,
			lock = lock_var,
			occup = occup_var,
			temp = temp_var
		}
	PATH = data_path
	send_rooms_db(json_data)
	end
end


function get_rooms_firebase(data_path, use_var, light_var, lock_var, occup_var, temp_var)    
	ip = wifi.sta.getip()

	if(ip==nil)then
		print("No connection established")
	else
		tmr.stop(0)
		json_data = {
			use = use_var,
			light = light_var,
			lock = lock_var,
			occup = occup_var,
			temp = temp_var
		}
	PATH = data_path
	get_rooms_db(json_data)
	end
end


function send_users_db(data_table)

	json = "{\"firstName\" : "..data_table["fname"]..",".."\"lastName\" : "..data_table["lname"]..",".."\"password\" : "..data_table["pass"]..",".."\"userName\" : "..data_table["user"].."}"	
		--	"\"roomNum\" : "..data_table["roomNum"]..", "..
		--	"\"occupied\" : "..data_table["occupancy"]..", "..
		--	"\"temperature\" : "..data_table["temp"].."}"
		
	
	data = {
	--path = "/DefaultRoomId",
    data = json
	}
	
	--path = "/DefaultRoomId.json"
	
	socket = net.createConnection(net.TCP,0)
	socket:on("receive", display)
    socket:connect(80,HOST_DB)
	
	socket:on("connection", function(sck)
	post_request = build_database_request(HOST_DB, URI_USER, data, PATH)
	sck:send(post_request)
    end)
end


function get_users_firebase()

	json = "{\"firstName\" : "..data_table["fname"]..",".."\"lastName\" : "..data_table["lname"]..",".."\"password\" : "..data_table["pass"]..",".."\"userName\" : "..data_table["user"].."}"	
		--	"\"roomNum\" : "..data_table["roomNum"]..", "..
		--	"\"occupied\" : "..data_table["occupancy"]..", "..
		--	"\"temperature\" : "..data_table["temp"].."}"
		
	
	data = {
	--path = "/DefaultRoomId",
    data = json
	}
	
	--path = "/DefaultRoomId.json"
	
	socket = net.createConnection(net.TCP,0)
	socket:on("receive", display)
    socket:connect(80,HOST_DB)
	
	socket:on("connection", function(sck)
	post_request = get_database_request(HOST_DB, URI_USER, data, PATH)
	sck:send(post_request)
    end)
end


function check_wifi()
  ip = wifi.sta.getip()

 if(ip==nil) then
   print("Connecting...")
 else
  tmr.stop(0)
  print("Connected to AP!")
  print(ip)
  
 end
end
