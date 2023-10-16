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

			// Modellbezeichung
			const part_name = clone.querySelector(".part_name");
			part_name.textContent = sensor.part_name

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
			modal_description.textContent = sensor.description;

			// Min/Max text
			const modal_inverval = clone.querySelector('.interval');
			modal_inverval.textContent = "Messurment between: " + sensor.min_interval +
			"-" + sensor.max_interval + " milliseconds";

			// Min/Max input field
			const modal_inverval_input = clone.querySelector('.interval_input');
			modal_inverval_input.setAttribute('min', sensor.min_interval);
			modal_inverval_input.setAttribute('max', sensor.max_interval);
			
			// Füge die bearbeitete Vorlage dem Dokument hinzu
			document.getElementById("sensor-grid").appendChild(clone);
		}
	}
	else {
		console.log("There is no 'sensor' key in the provided data");
	}
}

function download(filename, text) {
	const fileBlob = new Blob([text], { type: 'application/octet-binary' })
	const url = URL.createObjectURL(fileBlob)
  
	const link = document.createElement('a')
	link.setAttribute('href', url)
	link.setAttribute('download', filename)
  
	if (document.createEvent) {
	  const event = document.createEvent('MouseEvents')
	  event.initEvent('click', true, true)
	  link.dispatchEvent(event)
	} else {
	  link.click()
	}
  
	// Deallocate resources
	if (URL.revokeObjectURL)
	  URL.revokeObjectURL(url)
  }

function makeCsv(values) {
	let result = '';
	for (const arr of values) {
		for (let i = 0; i < arr.length; i++) {
			result += arr[i].toString();
			if (i != arr.length - 1) {
				result += ',';
			} else {
				result += '\n';
			}
		}
	}
	return result;
}

function onDownload() {
	fetch('/pres_get_imu_samples')
		.then(response => response.arrayBuffer())
		.then(arrayBuffer => {
			let view = new DataView(arrayBuffer);
			let result = [];
			for (let offset = 0; offset < view.byteLength; offset += 24) {
				result.push([
					view.getFloat32(offset, true),
					view.getFloat32(offset +  4, true),
					view.getFloat32(offset +  8, true),
					view.getFloat32(offset + 12, true),
					view.getFloat32(offset + 16, true),
					view.getFloat32(offset + 20, true),
				]);
			}
			const fmtSel = document.getElementById('download-format-select');
			switch (fmtSel.selectedIndex) {
				case 0:
					alert('Please select a format first.');
					break;
				case 1:
					download('data.csv', makeCsv(result));
					break;
				case 2:
					download('data.json', JSON.stringify(result));
					break;
			}
		})
}
