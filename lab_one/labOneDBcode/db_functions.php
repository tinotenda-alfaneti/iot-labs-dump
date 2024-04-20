<?php


class DataBaseFunctions
{
    private $con;

    // Constructor to initialize the $con variable
    function __construct($con)
    {
        $this->con = $con;
    }

    function inset($sensorName, $sensorType, $currentReading, $sensorLocation, $dateRead, $timeRead) {

          // Escape and quote the string values
        $sensorName = mysqli_real_escape_string($this->con, $sensorName);
        $sensorType = mysqli_real_escape_string($this->con, $sensorType);

        // Construct the SQL query with proper quoting
        $query = mysqli_query($this->con, "SELECT sensorID FROM Sensor WHERE sensorName = '$sensorName' AND sensorType = '$sensorType'");

        $data = mysqli_fetch_object($query);

        if ($data) {
            $fkID = $data->sensorID;
        } else {
            //simple use of prepared statements
            $sql='INSERT INTO Sensor (SensorName, SensorType) VALUES (?, ?)';
            $stmt = mysqli_prepare($this->con, $sql);
            mysqli_stmt_bind_param($stmt, "ss", $sensorName, $sensorType);

            $res=mysqli_stmt_execute($stmt);
            $fkID = $this->con->insert_id;

        }
        // simple use of prepared statements
        $sql='INSERT INTO SensorData (sensorID, currentReading, sensorLocation, dateRead, timeRead) VALUES (?, ?, ?, ?, ?)';
        $stmt = mysqli_prepare($this->con, $sql);
        mysqli_stmt_bind_param($stmt, "issss", $fkID, $currentReading, $sensorLocation, $dateRead, $timeRead);

        $res=mysqli_stmt_execute($stmt);
        return $res;
    }

    function delete($sensorDataID) {

        $sql = 'DELETE FROM sensordata WHERE SensorDataID=?';
  
        $stmt = mysqli_prepare($this->con, $sql);
        mysqli_stmt_bind_param($stmt, "i", $sensorDataID);
        $res=mysqli_stmt_execute($stmt);
        return $res;
    }

    function update($id, $currentReading, $sensorLocation, $dateRead, $timeRead) {

        $sql='UPDATE sensordata SET currentReading=?, sensorLocation=?, dateRead=?, timeRead=? WHERE SensorDataID=?';
        $stmt = mysqli_prepare($this->con, $sql);
        mysqli_stmt_bind_param($stmt, "sssss", $currentReading, $sensorLocation, $dateRead, $timeRead, $id);
        
        $res=mysqli_stmt_execute($stmt);

        return $res;
    }
    //--GET--//
    function get_data($id = null) {
        
        if ($id !== null) {
            // Fetch a specific item by ID
            $query = mysqli_query($this->con, "SELECT * FROM sensordata WHERE SensorDataID = $id");
            $data = mysqli_fetch_assoc($query);
            
            if ($data) {
                return json_encode($data);
            } else {
                return json_encode(['error' => 'Item not found']);
            }

        } else {
            $query = mysqli_query($this->con, 'SELECT * FROM sensordata');
            $data = [];

            while ($row = mysqli_fetch_object($query)) {
                $data[] = $row;
            }
            return $data;
        }


    }
	

}

?>