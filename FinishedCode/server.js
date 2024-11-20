const WebSocket = require('ws');

// WebSocket server settings
const PORT = 8888;
const wss = new WebSocket.Server({ port: PORT });

// When a client connects
wss.on('connection', (ws) => {
  console.log('New ESP32 connected');

  // Handle incoming messages from the client
  ws.on('message', (message) => {
    console.log('Received:', message);

    // Example: Save log to a file (optional)
    saveLogToFile(message);
  });

  // When a client disconnects
  ws.on('close', () => {
    console.log('ESP32 disconnected');
  });
});

// Function to save logs to a file
const fs = require('fs');
function saveLogToFile(log) {
  const logEntry = `${new Date().toISOString()} - ${log}\n`;
  fs.appendFile('logs.txt', logEntry, (err) => {
    if (err) {
      console.error('Error writing log to file:', err);
    }
  });
}

console.log(`WebSocket server running on ws://localhost:${PORT}`);
