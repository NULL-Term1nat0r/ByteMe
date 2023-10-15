document.addEventListener("DOMContentLoaded", function() {
	// Your JavaScript code here
	fetchData();
});

function fetchData() {
	// Mit der fetch()-Funktion wird eine Anfrage an den Server gesendet, um Daten abzurufen.
	fetch('sensor_info.json') // Hier sollte der Pfad zum Abrufen der Sensordaten sein
		.then(response => response.json()) // Die empfangenen Daten werden als JSON verarbeitet.
		.then(data => {
			// Aktualisieren Sie Ihre Webseite mit den empfangenen Daten.
			// In diesem Fall aktualisieren wir den Inhalt des HTML-Elements mit der ID "sensor-value".
			create_sensor_widget(data)
		})
		.catch(error => {
			console.error('Fehler beim Abrufen der Sensordaten:', error);
		});
}

function create_sensor_widget(data) {
	if ('sensors' in data) {
		let widget_count = data.sensors.length
		for (const sensor of data.sensors) {
			const template = document.getElementById("sensor-card-template");
			const clone = document.importNode(template.content, true);

			// Auf das Element mit der Klasse "template-name" in der geklonten Vorlage zugreifen
			const heading = clone.querySelector(".card-title");

			heading.textContent = sensor.name;
			
			// Füge die bearbeitete Vorlage dem Dokument hinzu
			document.getElementById("sensor-grid").appendChild(clone);
		}
	}
	else {
		console.log("There is no 'sensor' key in the provided data");
	}
}

function test_fill() {
	for (let i = 0; i < 5; i++) {
		// Vorlage klonen
		const template = document.getElementById("sensor-card-template");
		const clone = document.importNode(template.content, true);

		// Auf das Element mit der Klasse "template-name" in der geklonten Vorlage zugreifen
		const heading = clone.querySelector(".card-title");

		heading.textContent = "WUWUWUWU";
		
		// Füge die bearbeitete Vorlage dem Dokument hinzu
		document.body.appendChild(clone);
	}
}
