for proper output display, Window Screen Setting should be as follow, 

![image](https://github.com/MaheshYTech/db/assets/53365208/3c01f07d-d5aa-485a-af58-fe5a47e8f1af)


Download  [db.7z]

create folder [db]

extract files from [db.7z] into [db] folder


which contain following  files and other files.
1. Database.exe
2. stockfeb.txt
3. stockfeb.csv
4. car.txt
5. car.csv

stockfeb.txt and car.txt are sample files.

all  files should be in same foulder. 
Double Click on [Database.exe]

   ![image](https://github.com/MaheshYTech/db/assets/53365208/089ae1dc-99b0-4a76-af57-5e8562c513f4)

For Create Database , Select Option [1]


![image](https://github.com/MaheshYTech/db/assets/53365208/b0d25db6-d3ba-4b4a-b60e-daa331944aa5)

select option [2] . has to open database.

![image](https://github.com/MaheshYTech/db/assets/53365208/9ecbea9f-c76c-4539-a0d2-a880b1df9267)


if database available it shows Structure of Database

![image](https://github.com/MaheshYTech/db/assets/53365208/68e00307-bc47-463b-bd7b-00228e2bad49)

Now [car.txt] has been openend

To view database content select option [6] which display records.
![image](https://github.com/MaheshYTech/db/assets/53365208/d396d5f8-0eca-4db2-94b3-fe32c8b00e31)

To [IMPORT] csv file directly, select option [4]
[4. Import From Files]

![image](https://github.com/MaheshYTech/db/assets/53365208/50f00167-72aa-4c4a-93b1-0f09771e8a4e)


To run [SQL QUERY] select option [7]

Query is 
select * from car.txt where cyly >= 4 and cyly <= 6
select * from car.txt where cyly = 4

![image](https://github.com/MaheshYTech/db/assets/53365208/8e8ee260-bf3a-4879-b0ed-13965ea9a98f)

Query Result for above Query is
![image](https://github.com/MaheshYTech/db/assets/53365208/f94470d1-935a-4a49-9a40-6130e59de008)

Delete Query
delete from car.txt where cyly = 4 or cyly = 6
delete from car.txt where cyly <= 6
if you run below [query[
delete from car.txt where cyly = 4

![image](https://github.com/MaheshYTech/db/assets/53365208/97cc90b9-3b07-4f24-9848-d449d82bef70)


Now if we run SELECT query  , 4 cyly  has been deleted.

![image](https://github.com/MaheshYTech/db/assets/53365208/4be05b18-2675-42f0-ad40-141eb8154456)



