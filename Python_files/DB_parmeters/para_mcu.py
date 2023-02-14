import psycopg2
from datetime import datetime

conn = psycopg2.connect(database = "parameters", user = "postgres", password = "postgresql", host = "127.0.0.1", port = "5432")
print ("Opened database successfully")

cur = conn.cursor()
# datetime object containing current date and time
now = datetime.now()

cur.execute('''CREATE TABLE Mcu_parameters
      (POWER               FLOAT,
      CLOCKFREQUENCY       INT,
      CURRENT              FLOAT,
      VOLTAGE              FLOAT,
      TEMPERATURE          FLOAT,
      PAYLOAD              INT,
      EXACT_TIME           TIMESTAMP);''')
print ("Table created successfully")
# dd/mm/YY H:M:S
dt_string = now.strftime(" Date:%d/%m/%Y,Time:%H:%M:%S")
print(dt_string)

conn.commit()
conn.close()