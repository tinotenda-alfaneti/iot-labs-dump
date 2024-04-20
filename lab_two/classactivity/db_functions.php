<?php


class DataBaseFunctions
{
    private $con;

    // Constructor to initialize the $con variable
    function __construct($con)
    {
        $this->con = $con;
    }

    function inset($temperatureVal, $humidityVal, $groupName, $pumpStatus, $collectionTime) {

          // Escape and quote the string values
        $groupName = mysqli_real_escape_string($this->con, $groupName);
        $pumpStatus = mysqli_real_escape_string($this->con, $pumpStatus);
        $collectionTime = mysqli_real_escape_string($this->con, $collectionTime);

        // Construct the SQL query with proper quoting
      
        $sql='INSERT INTO datacollection (temperatureVal, humidityVal, groupName, pumpStatus, collectionTime) VALUES (?, ?, ?, ?, ?)';
        $stmt = mysqli_prepare($this->con, $sql);
        mysqli_stmt_bind_param($stmt, "disss", $temperatureVal, $humidityVal, $groupName, $pumpStatus, $collectionTime);

        $res=mysqli_stmt_execute($stmt);
        return $res;
    }

    function get_all_data() {
        $query = mysqli_query($this->con, 'SELECT * FROM datacollection');
    
        $data = [];
    
        while ($row = mysqli_fetch_object($query)) {
            $data[] = $row;
        }
    
        return $data;
    }

    function get_last_records() {
        
        // $query = mysqli_query($this->con, 'SELECT * FROM datacollection ORDER BY dataID DESC LIMIT 10');
        $query = mysqli_query($this->con, 'SELECT * FROM (SELECT * FROM datacollection ORDER BY dataID DESC LIMIT 10) AS subquery ORDER BY dataID ASC');
    
        $data = [];
    
        while ($row = mysqli_fetch_object($query)) {
            $data[] = $row;
        }
    
        return $data;
    }
	

}

?>