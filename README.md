# LinkKeywordCounter
Displays the number of links found for each URL in a supplied text file along with the href value given a specified keyword.   
Useful for seeing trends in data such as news headlines or current events.

Required OS: Linux

### Using & Running:
* In a command line pointing to the project directory, "make all", then "make run". 
* Output will be given by a new text file in the project directory with the name of "output.txt".
* Each line of the text file represents an occurrence of the keyword with the URL 
#### Known limitations:
curl is typically not included by default on most machines causing a fatal error when running. "sudo apt-get install libcurl4-nss-dev" will fix this.
This tool cannot bypass websites utilizing paywalls!!!  
Some URLs may be listed twice. This is due to images containing href values along with a caption underneath with the same value causing them to be counted twice.
