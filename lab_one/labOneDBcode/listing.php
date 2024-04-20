<?php

include "config.php";


class Listings
{
    private $con;

    // Constructor to initialize the $con variable
    function __construct($con)
    {
        $this->con = $con;
    }

    function get_all_data() {

        $query = mysqli_query($this->con, 'SELECT * FROM sensordata');
        $data = [];

        while ($row = mysqli_fetch_object($query)) {
            $data[] = $row;
        }
        return $data;

    }

    function get_temperature_above_35() {
        
        $query1 = mysqli_query($this->con, "SELECT sensorID FROM Sensor WHERE sensorType = 'Temperature'");
        $sensorIDs = [];
    
        while ($data1 = mysqli_fetch_object($query1)) {
            $sensorIDs[] = $data1->sensorID;
        }
    
        $data = [];
    
        foreach ($sensorIDs as $sensorID) {
            $query2 = mysqli_query($this->con, "SELECT * FROM SensorData WHERE sensorID = $sensorID");
    
            while ($row = mysqli_fetch_object($query2)) {
                if (intval($row->currentReading) > 35) {
                    $data[] = $row;
                }
                
            }
        }
        return json_encode($data);
    }


    function get_temperature() {
        
        $query1 = mysqli_query($this->con, "SELECT sensorID FROM Sensor WHERE sensorType = 'Temperature'");
        $sensorIDs = [];
    
        while ($data1 = mysqli_fetch_object($query1)) {
            $sensorIDs[] = $data1->sensorID;
        }
    
        $data = [];
    
        foreach ($sensorIDs as $sensorID) {
            $query2 = mysqli_query($this->con, "SELECT * FROM SensorData WHERE sensorID = $sensorID");
    
            while ($row = mysqli_fetch_object($query2)) {
                $data[] = $row;
            }
        }
        return json_encode($data);
    }

    
    //--GET--//
    function get_particular_record($id) {
        
        $query = mysqli_query($this->con, "SELECT * FROM sensordata WHERE SensorDataID = $id");
        $data = mysqli_fetch_assoc($query);
        
        if ($data) {
            return json_encode($data);
        } else {
            return json_encode(['error' => 'Item not found']);
        }


    }
	

}


?>