<?php


include "config.php";

class DataBaseClass
{
    private $con;

    // Constructor to initialize the $con variable
    function __construct($con)
    {
        $this->con = $con;
    }

	//--CREATE TABLE--//

    function create_tables() {
        $sql1 = "CREATE TABLE IF NOT EXISTS Sensor (
                    sensorID INT AUTO_INCREMENT PRIMARY KEY,
                    sensorName VARCHAR(255) NOT NULL,
                    sensorType VARCHAR(255)
                )";
    
        $stmt1 = mysqli_prepare($this->con, $sql1);
        $res1 = mysqli_stmt_execute($stmt1);
    
        if (!$res1) {
            echo "Error in Sensor table creation: " . mysqli_error($this->con);
            return false;
        }
    
        $sql2 = "CREATE TABLE IF NOT EXISTS SensorData (
                    sensorDataID INT AUTO_INCREMENT PRIMARY KEY,
                    sensorID INT NOT NULL,
                    currentReading VARCHAR(255) NOT NULL,
                    sensorLocation VARCHAR(255),
                    dateRead VARCHAR(255),
                    timeRead VARCHAR(255),
                    FOREIGN KEY (sensorID) REFERENCES Sensor(sensorID)
                )";
    
        $stmt2 = mysqli_prepare($this->con, $sql2);
        $res2 = mysqli_stmt_execute($stmt2);
    
        if (!$res2) {
            echo "Error in SensorData table creation: " . mysqli_error($this->con);
            return false;
        }
    
        return true;
    }
	

}


?>