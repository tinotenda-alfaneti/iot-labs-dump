<?php


include "config.php";
include "create_tables.php";
include "db_functions.php";
include "listing.php";

$tableName = "sensordata";
$db_class = new DataBaseClass($con);
$db_functions = new DataBaseFunctions($con);
$db_listings = new Listings($con);
$db_class->create_tables();




// Handle HTTP methods
$method = $_SERVER['REQUEST_METHOD'];
switch ($method) {

  case 'GET':
    $requestUri = $_SERVER['REQUEST_URI'];

    // Check if it's a request for all items or a specific item
    if (strpos($requestUri, '/record/') !== false) {
        // Extract the item ID from the URL
        
        $id = intval(basename($requestUri));

        $data = $db_listings->get_particular_record($id);
        
        echo $data;
        if ($data) {
          http_response_code(200);
          return $data;
      } else {
          http_response_code(404);
          return json_decode(['error' => 'Item not found']);
      }
        
    } else if (strpos($requestUri, '/temperature') !== false) {

      $data = $db_listings->get_temperature();
        echo $data;
        if ($data) {
          http_response_code(200);
          return $data;
      } else {
          http_response_code(404);
          return json_decode(['error' => 'Item not found']);
      }

    } else if (strpos($requestUri, '/temp_35') !== false) {

      $data = $db_listings->get_temperature_above_35();
        echo $data;
        if ($data) {
          http_response_code(200);
          return $data;
      } else {
          http_response_code(404);
          return json_decode(['error' => 'Item not found']);
      } 
    } else {
        // Request for all items
        $data = $db_listings->get_all_data();

        http_response_code(200);
        echo json_encode($data);
    }
    break;

  case 'POST':
  // Create operation (add a new sensorData)

  $data = json_decode(file_get_contents('php://input'), true);
  $sensorName = $data['SensorName'];
  $sensorType = $data['SensorType'];
  $currentReading = $data['CurrentReading'];
  $sensorLocation = $data['SensorLocation'];
  $dateRead = $data['DateRead'];
  $timeRead = $data['TimeRead'];
  

  $res = $db_functions->inset($sensorName, $sensorType, $currentReading, $sensorLocation, $dateRead, $timeRead);
  if ($res)
    echo json_encode(['message' => 'SensorData added successfully...']);
  else 
    echo json_encode(['message' => 'Failure to add data']);
  break;
  
  case 'PUT':
  // Update operation (edit a record)
  $data = json_decode(file_get_contents('php://input'), true); //true means return as assoc arrat. false means return object
  
  $id = $_GET['id'];//passed as a parameter in the URL

  $currentReading = $data['CurrentReading'];
  $sensorLocation = $data['SensorLocation'];
  $dateRead = $data['DateRead'];
  $timeRead = $data['TimeRead'];
  
  $res=$db_functions->update($id, $currentReading, $sensorLocation, $dateRead, $timeRead);

  if ($res !== FALSE)
    echo json_encode(['message' => 'SensorData updated successfully...']);
  else 
    echo json_encode(['message' => 'Failure to update']);
  break;


  case 'DELETE':
  // Delete operation (remove a record)
  $id = $_GET['id'];
  
  $res=$db_functions->delete($id);
  if ($res)
    echo json_encode(['message' => 'SensorData deleted successfully...']);
  else 
    echo json_encode(['message' => 'Failure to delete']);
  break;
  default:
  // Invalid method
  http_response_code(405);
  echo json_encode(['error' => 'Method not allowed']);
  break;
}

?>