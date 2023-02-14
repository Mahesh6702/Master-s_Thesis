import psycopg2
from datetime import datetime, timezone



conn = psycopg2.connect(database = "parameters", user = "postgres", password = "postgresql", host = "127.0.0.1", port = "5432")
print ("Opened database successfully")

paragraph = ' 400 || 255 || 255 || 200 '
# Split the text wherever there's a ||.
CLOCKFREQUENCY, CURRENT,POWER,TEMPERATURE = paragraph.split('||')
print(CLOCKFREQUENCY);
#print(PERIPHERALS);
print(CURRENT);
print(POWER);
print(TEMPERATURE);

cur = conn.cursor()
# datetime object containing current date and time
now = datetime.now()
dt = datetime.now(timezone.utc)
#cur.execute("INSERT INTO Mcu_parameters (POWER ,CLOCKFREQUENCY,CURRENT,VOLTAGE,TEMPERATURE,PAYLOAD) \
 #     VALUES (540, '400', 150, '5', '28.5','154' )");
cur.execute('INSERT INTO Mcu_parameters(EXACT_TIME) VALUES (%s)', (dt,))
cur.execute('INSERT INTO Mcu_parameters(CLOCKFREQUENCY) VALUES (%s)', (CLOCKFREQUENCY,))
#cur.execute('INSERT INTO Mcu_parameters(PERIPHERALS) VALUES (%s)', (PERIPHERALS,))
cur.execute('INSERT INTO Mcu_parameters(CURRENT) VALUES (%s)', (CURRENT,))
cur.execute('INSERT INTO Mcu_parameters(POWER) VALUES (%s)', (POWER,))
cur.execute('INSERT INTO Mcu_parameters(TEMPERATURE) VALUES (%s)', (TEMPERATURE,))



conn.commit()

dt_string = now.strftime(" Date:%d/%m/%Y,Time:%H:%M:%S")
print(dt_string)

print ("Records created successfully")
conn.close()