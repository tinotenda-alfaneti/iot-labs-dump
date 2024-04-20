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
        $sql = "CREATE TABLE IF NOT EXISTS datacollection (
                    dataID INT AUTO_INCREMENT PRIMARY KEY,
                    recordedTime TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
                    temperatureVal FLOAT(4,2),
                    humidityVal INT,
                    groupName VARCHAR(30),
                    pumpStatus VARCHAR(4),
                    collectionTime VARCHAR(100)

                )";
    
        $stmt = mysqli_prepare($this->con, $sql);
        $res = mysqli_stmt_execute($stmt);
    
        if (!$res) {
            echo "Error in datacollection table creation: " . mysqli_error($this->con);
            return false;
        }
    
        return true;
    }
	

}


?>