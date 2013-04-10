myLat	= 42.352271;  // These are the coordinates of South Station, which acts as
myLng	= -71.055242; // a good start point for the program
var scheduleRequest	= new XMLHttpRequest();
var wcRequest		= new XMLHttpRequest();
var myLoc	= new google.maps.LatLng(myLat, myLng);
var options	= {
	zoom: 13,
	center: myLoc,
	mapTypeId: google.maps.MapTypeId.ROADMAP
	};
var map;
var myMarker; //holds the marker of the user
var waldo;
var carmen;
var stations		= []; //array to hold all the station markers
var stationKeys		= []; //array to hold corresponding platform keys of stations
var redTrunk		= []; //holds the markers that all red line trains use
var ashmontPath 	= []; //holds the markers that only Ashmont trains use
var braintreePath	= []; //holds the markers that only Braintree trains use 
var myInfoWindow	= new google.maps.InfoWindow();
var stationInfoWindow = new google.maps.InfoWindow();

function start(){
	map = new google.maps.Map(document.getElementById("map_canvas"), options);
	renderMap();
}

function renderMap(){
	renderRedLine();
	getMyLoc();
}

function renderRedLine(){
	createStationMarkers();
	correspondingKeys = new Object();

	for (i = 0; i < stations.length; i++){
		correspondingKeys[stations[i]["title"]] = stationKeys[i];
	}
	
	for (var i in stations){
		google.maps.event.addListener(stations[i], "click", function() {
			marker = this;
			key    = correspondingKeys[this["title"]]
			scheduleRequest.onreadystatechange = getSchedule;	
			scheduleRequest.open("GET", "http://mbtamap.herokuapp.com/mapper/redline.json ",
						 		true);
			scheduleRequest.send(null);
			
			stationInfoWindow.open(map, marker);			
		});
		stations[i].setMap(map);
	}
	renderPolylines();
}

function getSchedule(){
	if (this.readyState == 4){
		if (this.status == 200){	
			var contents 	= "<h2>" + marker["title"] + "</h2>";
			var stationKey  = correspondingKeys[marker["title"]];
			var str			= scheduleRequest.responseText;
			var findStation = new RegExp (stationKey);
			var northbound  = new RegExp ("N$");
			var southbound  = new RegExp ("S$");
			var schedule	= JSON.parse(str);
	
			for (i = 0; i < schedule.length; i ++){
				if ((findStation.test(schedule[i]["PlatformKey"])) && 
					(schedule[i]["InformationType"] != "Arrived")){
										
					if (northbound.test(schedule[i]["PlatformKey"])){
						contents += "<p>Northbound T arriving in: " + 
									schedule[i]["TimeRemaining"] + " minutes.</p>";
					}
					else{
						contents += "<p>Southbound T arriving in: " + 
									schedule[i]["TimeRemaining"] + " minutes.</p>";
					}	
				}	
			}
			stationInfoWindow.setContent(contents);
		}
	}
}					

function getMyLoc(){
	if (navigator.geolocation) { 
		navigator.geolocation.getCurrentPosition(function(position) {
			myLat = position.coords.latitude;
			myLng = position.coords.longitude;
			placeMyLoc();
			findClosestStation();
			findWaldoAndCarmen();
		});
	}
	else {
		alert("Geolocation is not supported by your browser. Try using a different one");
	}
}

function placeMyLoc(){
	myLoc = new google.maps.LatLng(myLat, myLng);
	map.panTo(myLoc);
	
	myMarker = new google.maps.Marker({
		position: myLoc,
		title: "You are here"
	});
	myMarker.setMap(map);
	google.maps.event.addListener(myMarker, "click", function() {
		var marker	= this;	
		str			= "You are here at " + myLat + ", " + myLng;
		myInfoWindow.setContent(str);
		myInfoWindow.open(map, marker);		
	});
}

function findClosestStation(){
	closest			= findDistance(myLat, myLng, stations[0]["position"].lat(),
								   stations[0]["position"].lng());
	closestStation	= stations[0];
	
	for (i = 1; i < stations.length; i++){
		test = findDistance(myLat, myLng, stations[i]["position"].lat(),
							stations[i]["position"].lng());
		if (test < closest){
			closest			= test;
			closestStation	= stations[i];
		}
	}
	closestInfo	= "<p>You are here. The closest station is " + closestStation["title"] + 
				  ", which is approximately " + closest + " miles away. </p>";
	myInfoWindow.setContent(closestInfo);
	myInfoWindow.open(map, myMarker);

	var closestPath = new Array();
	closestPath.push(myMarker["position"]);
	closestPath.push(closestStation["position"]);

	closestLine = new google.maps.Polyline({
		path: closestPath,
		strokeColor: "#000000",
		strokeOpacity: 0.6,
		strokeWeight: 5
	});
	closestLine.setMap(map);

}

//uses the Haversine Formula to find the distance between two points, taken from
//http://www.movable-type.co.uk/scripts/latlong.html
function findDistance(lat1, lon1, lat2, lon2){
	Number.prototype.toRad = function() {
		return this * Math.PI / 180;
	}
	var R	 = 6371; // km
	var dLat = (lat2 - lat1).toRad();
	var dLon = (lon2 - lon1).toRad();
	var lat1 = lat1.toRad();
	var lat2 = lat2.toRad();

	var a = Math.sin(dLat/2) * Math.sin(dLat/2) +
    		Math.sin(dLon/2) * Math.sin(dLon/2) * Math.cos(lat1) * Math.cos(lat2); 
	var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a)); 
	var d = R * c;
	
	d /= 1.609344; //converts the result to miles
	return d;
}

function findWaldoAndCarmen(){
	wcRequest.open("GET", "http://messagehub.herokuapp.com/a3.json", true);
	wcRequest.send(null);
	wcRequest.onreadystatechange = callback;
}

function callback(){
	if (wcRequest.readyState == 4){
		if (wcRequest.status == 200){
			var str = wcRequest.responseText;
			waldoCarmenInfo = JSON.parse(str);
			placeWaldoAndCarmen(waldoCarmenInfo);
		}
		else{
			alert ("No locations received from AJAX.  Waldo and Carmen are in hiding!");
		}
	}
}

function placeWaldoAndCarmen(info){
	var marker;
	var waldoCarmenInfoWindow = new google.maps.InfoWindow();
	var waldoDistance  = 0;
	var carmenDistance = 0;

	for (i = 0; i < info.length; i++){
		if (info[i]["name"] == "Waldo"){
			pos	 = new google.maps.LatLng(info[i]["loc"]["latitude"],
										  info[i]["loc"]["longitude"]);
			waldoTxt = info[i]["loc"]["note"];
			pic		 = "images/waldo.png";
			waldo	 = new google.maps.Marker(
				{position: pos, title: info[i]["name"], icon: pic});
			google.maps.event.addListener(waldo, "click", function() {
				waldoCarmenInfoWindow.setContent(waldoTxt);
				waldoCarmenInfoWindow.open(map, waldo);			
			});
			waldo.setMap(map);
			waldoDistance = findDistance(myLoc.lat(), myLoc.lng(), pos.lat(), pos.lng());
		}
		else{
			pos	 = new google.maps.LatLng(info[i]["loc"]["latitude"],
										  info[i]["loc"]["longitude"]);
			carmenTxt = info[i]["loc"]["note"];
			pic		  = "images/carmen.png";
			carmen	  = new google.maps.Marker(
				{position: pos, title: info[i]["name"], icon: pic});
			google.maps.event.addListener(carmen, "click", function() {
				waldoCarmenInfoWindow.setContent(carmenTxt);
				waldoCarmenInfoWindow.open(map, carmen);			
			});
			carmen.setMap(map);
			carmenDistance = findDistance(myLoc.lat(), myLoc.lng(), pos.lat(), pos.lng());
		}
	}
	
	list = document.getElementById("waldo_carmen_info");
	if (waldoDistance != 0){
		waldoNode = document.createElement("li");
		waldoText = document.createTextNode("Waldo is " + waldoDistance + " miles away ")
	 	waldoNode.appendChild(waldoText);
	 	list.appendChild(waldoNode);
	}
	if (carmenDistance != 0){
		carmenNode = document.createElement("li");
		carmenText = document.createTextNode("Carmen Sandiego is " + carmenDistance + 
											 " miles away")
	 	carmenNode.appendChild(carmenText);
	 	list.appendChild(carmenNode);
	}
}

//creates all the stations markers and stores them in the arrays, but does not render them
function createStationMarkers(){
	tIcon = "images/tIcon.png";

	stop = new google.maps.LatLng(42.395428, -71.142483);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Alewife Station", icon: tIcon}));
	stationKeys.push("RALE");
	redTrunk.push(stop);
		
	stop = new google.maps.LatLng(42.39674, -71.121815);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Davis Station", icon: tIcon}));
	stationKeys.push("RDAV");
	redTrunk.push(stop);
		
	stop = new google.maps.LatLng(42.3884, -71.119149);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Porter Square Station", icon: tIcon}));
	stationKeys.push("RPOR");
	redTrunk.push(stop);
		
	stop = new google.maps.LatLng(42.373362, -71.118956);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Harvard Square Station", icon: tIcon}));
	stationKeys.push("RHAR");
	redTrunk.push(stop);
		
	stop = new google.maps.LatLng(42.365486, -71.103802);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Central Square Station", icon: tIcon}));
	stationKeys.push("RCEN");
	redTrunk.push(stop);
		
	stop = new google.maps.LatLng(42.36249079, -71.08617653);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Kendall/MIT Station", icon: tIcon}));
	stationKeys.push("RKEN");
	redTrunk.push(stop);
		
	stop = new google.maps.LatLng(42.361166, -71.070628);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Charles/MGH Station", icon: tIcon}));
	stationKeys.push("RMGH");
	redTrunk.push(stop);
		
	stop = new google.maps.LatLng(42.35639457, -71.0624242);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Park St. Station", icon: tIcon}));
	stationKeys.push("RPRK");
	redTrunk.push(stop);
		
	stop = new google.maps.LatLng(42.355518, -71.060225);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Downtown Crossing Station", icon: tIcon}));
	stationKeys.push("RDTC");
	redTrunk.push(stop);
		
	stop = new google.maps.LatLng(42.352271, -71.055242);
	stations.push(new google.maps.Marker(
		{position: stop, title: "South Station", icon: tIcon}));
	stationKeys.push("RSOU");
	redTrunk.push(stop);
		
	stop = new google.maps.LatLng(42.342622, -71.056967);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Broadway Station", icon: tIcon}));
	stationKeys.push("RBRO");
	redTrunk.push(stop);
		
	stop = new google.maps.LatLng(42.330154, -71.057655);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Andrew Station", icon: tIcon}));
	stationKeys.push("RAND");
	redTrunk.push(stop);
		
	stop = new google.maps.LatLng(42.320685, -71.052391);
	stations.push(new google.maps.Marker(
		{position: stop, title: "JFK/UMass Station", icon: tIcon}));
	stationKeys.push("RJFK");
	redTrunk.push(stop);
	ashmontPath.push(stop);
	braintreePath.push(stop);
		
	stop = new google.maps.LatLng(42.31129, -71.053331);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Savin Hill Station", icon: tIcon}));
	stationKeys.push("RSAV");
	ashmontPath.push(stop);
		
	stop = new google.maps.LatLng(42.300093, -71.061667);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Fields Corner Station", icon: tIcon}));
	stationKeys.push("RFIE");
	ashmontPath.push(stop);
	
	stop = new google.maps.LatLng(42.29312583, -71.06573796);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Shawmut Station", icon: tIcon}));
	stationKeys.push("RSHA");
	ashmontPath.push(stop);
	
	stop = new google.maps.LatLng(42.284652, -71.064489);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Ashmont Station", icon: tIcon}));
	stationKeys.push("RASH");
	ashmontPath.push(stop);
	
	stop = new google.maps.LatLng(42.275275, -71.029583);
	stations.push(new google.maps.Marker(
		{position: stop, title: "North Quincy Station", icon: tIcon}));
	stationKeys.push("RNQU");
	braintreePath.push(stop);
		
	stop = new google.maps.LatLng(42.2665139, -71.0203369);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Wollaston Station", icon: tIcon}));
	stationKeys.push("RWOL");
	braintreePath.push(stop);
		
	stop = new google.maps.LatLng(42.251809, -71.005409);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Quincy Center Station", icon: tIcon}));
	stationKeys.push("RQUC");
	braintreePath.push(stop);
		
	stop = new google.maps.LatLng(42.233391, -71.007153);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Quincy Adams Station", icon: tIcon}));
	stationKeys.push("RQUA");
	braintreePath.push(stop);
	
	stop = new google.maps.LatLng(42.2078543, -71.0011385);
	stations.push(new google.maps.Marker(
		{position: stop, title: "Braintree Station", icon: tIcon}));
	stationKeys.push("RBRA");
	braintreePath.push(stop);
}


function renderPolylines(){
	redTrunkLine = new google.maps.Polyline({
		path: redTrunk,
		strokeColor: "#FF0000",
		strokeOpacity: 0.75,
		strokeWeight: 5
	});
	redTrunkLine.setMap(map);

	redAshmontLine = new google.maps.Polyline({
		path: ashmontPath,
		strokeColor: "#FF0000",
		strokeOpacity: 0.75,
		strokeWeight: 5
	});
	redAshmontLine.setMap(map);
	
	redBraintreeLine = new google.maps.Polyline({
		path: braintreePath,
		strokeColor: "#FF0000",
		strokeOpacity: 0.75,
		strokeWeight: 5
		});
	redBraintreeLine.setMap(map);
}
