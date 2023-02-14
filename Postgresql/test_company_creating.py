import psycopg2
from datetime import datetime

conn = psycopg2.connect(database = "dbname", user = "postgres", password = "postgresql", host = "127.0.0.1", port = "5432")
print ("Opened database successfully")

cur = conn.cursor()
# datetime object containing current date and time
now = datetime.now()

cur.execute('''CREATE TABLE SSE
      (ID INT PRIMARY KEY     NOT NULL,
      NAME           TEXT    NOT NULL,
      AGE            INT     NOT NULL,
      ADDRESS        CHAR(50),
      SALARY         REAL);''')
print ("Table created successfully")
# dd/mm/YY H:M:S
dt_string = now.strftime(" Date:%d/%m/%Y,Time:%H:%M:%S")
print(dt_string)

conn.commit()
conn.close()