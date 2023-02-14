import psycopg2
from datetime import datetime, timezone



conn = psycopg2.connect(database = "parameters", user = "postgres", password = "postgresql", host = "127.0.0.1", port = "5432")
print ("Opened database successfully")

cur = conn.cursor()
# datetime object containing current date and time
now = datetime.now()
dt = datetime.now(timezone.utc)
cur.execute("INSERT INTO sse (ID,NAME,AGE,ADDRESS,SALARY) \
      VALUES (1, 'Paul', 32, 'California', 20000.00 )");

cur.execute("INSERT INTO sse (ID,NAME,AGE,ADDRESS,SALARY) \
      VALUES (2, 'Allen', 25, 'Texas', 15000.00 )");

cur.execute("INSERT INTO sse (ID,NAME,AGE,ADDRESS,SALARY) \
      VALUES (3, 'Teddy', 23, 'Norway', 20000.00 )");

cur.execute("INSERT INTO sse (ID,NAME,AGE,ADDRESS,SALARY) \
      VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 )");
cur.execute('INSERT INTO sse(mycol) VALUES (%s)', (dt,))

conn.commit()

dt_string = now.strftime(" Date:%d/%m/%Y,Time:%H:%M:%S")
print(dt_string)

print ("Records created successfully")
conn.close()