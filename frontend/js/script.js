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
		for (const sensor of data.sensors) {
			const template = document.getElementById("sensor-card-template");
			const clone = document.importNode(template.content, true);

			// Überschrift Sensor Widget
			const heading = clone.querySelector(".card-title");
			heading.textContent = sensor.name;

			// Widget
			const modal = clone.querySelector('.sensor-modal');
			modal.setAttribute('id', "Sensor_id_" + sensor.uuid);

			// Button um das Widget zu öffnen
			const modal_button = clone.querySelector('.modal-button');
			modal_button.setAttribute('data-bs-target', "#" + modal.id);

			// Überschrift im geöffnetten Widget
			const modal_title = clone.querySelector('.modal-title');
			modal_title.textContent = sensor.name;

			// Beschreibungstext
			const modal_description = clone.querySelector('.sensor_description');
			modal_description.textContent = sensor.summary;
			console.log(sensor.summary)


			// const modal_description = clone.querySelector('.modal-body');
			// modal_description.setAttribute('id', )

			
			// Füge die bearbeitete Vorlage dem Dokument hinzu
			document.getElementById("sensor-grid").appendChild(clone);
		}
	}
	else {
		console.log("There is no 'sensor' key in the provided data");
	}
}
