{
	"@info" :
		{
			"gid" : "00000000000000000000000000000000",
			"name" : "plunMqttPub",
			"description" : "mqtt publisher",
			"author" : "Nsynapse Inc.",
			"author_email" : "bhhwang@nsynapse.com",
			"url" : "www.nsynapse.com",
			"license" : "BSD",
			"version" : "0.0.1",
			"category" : "system",
			"tags" : ["device", "host", "system"],
			"packages" : ["compMqttPub", "compSqlite"]
		},
		
	"@property" :
		{
			"mqtt_port" : 1883,
			"use_ethernet" : "eth0"
		}
}