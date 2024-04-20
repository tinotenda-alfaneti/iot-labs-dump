<?php


include "config.php";
include "create_tables.php";
include "db_functions.php";

$tableName = "sensordata";
$db_class = new DataBaseClass($con);
$db_functions = new DataBaseFunctions($con);
$db_class->create_tables();

// Handle HTTP methods
$method = $_SERVER['REQUEST_METHOD'];
switch ($method) {

  case 'GET':
    $requestUri = $_SERVER['REQUEST_URI'];

    // Check if it's a request for all items or a specific item
    if (strpos($requestUri, '/records') !== false) {
        // Extract the item ID from the URL

        $data = $db_functions->get_last_records();
        
        if ($data) {
          http_response_code(200);
          
      } else {
          http_response_code(404);
          return json_decode(['error' => 'Item not found']);
      }
        
    } else {
        // Request for all items
        $data = $db_functions->get_all_data();

        http_response_code(200);
    }
    ?>

<!DOCTYPE html>
<html>
<head>
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
</head>
<body>
<div class="container">
 <div class="row">
   <div class="col-sm-12">
    <div class="table-responsive">
      <table class="table table-bordered">
       <thead><tr><th>Data ID</th>
       <th>Pump Status</th>
         <th>Temperature Value</th>
         <th>Humidity Value</th>
         <th>Group Name</th>
         <th>Time Collected</th>
         
    </thead>
    <tbody>
  <?php
      if(is_array($data)){      
      $sn=1;
      foreach($data as $row){
    ?>
      <tr>
      <td><?php echo $row->dataID?? ''; ?></td>
      <td><?php echo $row->pumpStatus??''; ?></td> 
      <td><?php echo $row->temperatureVal??''; ?></td>
      <td><?php echo $row->humidityVal??''; ?></td>
      <td><?php echo $row->groupName??''; ?></td>
      <td><?php echo $row->recordedTime??''; ?></td>   
     </tr>
     <?php
      $sn++;}}else{ ?>
      <tr>
        <td colspan="12">
    <?php echo $data; ?>
  </td>
    <tr>
    <?php
    }?>
    </tbody>
     </table>
   </div>
</div>
</div>
</div>
</body>
</html>

    <?php
    break;

  case 'POST':
    // Create operation (add a new sensorData)

    $data = json_decode(file_get_contents('php://input'), true);
   
    $temperatureVal = $data['temperatureVal'];
    $humidityVal = $data['humidityVal'];
    $groupName = $data['groupName'];
    $pumpStatus = $data['pumpStatus'];
    $collectionTime = $data['collectionTime'];

    $res = $db_functions->inset($temperatureVal, $humidityVal, $groupName, $pumpStatus, $collectionTime);
    if ($res)
      echo json_encode(['message' => 'Data added successfully...']);
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

}

?>