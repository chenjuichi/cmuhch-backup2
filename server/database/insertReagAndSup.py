from tables import Supplier, Product, Reagent, Grid, Session

import pymysql
from sqlalchemy import exc

import faker
faker = faker.Faker(locale='zh_TW')

# --------------------------

# insert
s = Session()

# ---grid table data

g_station = ['1', '2', '3', '1', '2', '1', '2',
             '3', '1', '2', '2', '3', '1', '3', '1', '1', '2', '1', '1']  # 1 ~ 3
g_layout = ['4', '4', '1', '2', '3', '5', '4',
            '2', '3', '4', '2', '3', '1', '5', '4', '2', '3', '5', '3']  # 1 ~ 5
g_position = ['6', '1', '1', '2', '5', '6', '8',
              '1', '2', '5', '2', '3', '1', '10', '1', '1', '1', '1', '1']  # 1 ~ 10

#               1     2    3     4     5     6     7     8     9     10    11    12    13    14
g_led_seg_id = ['6',  '1', '1',  '2',  '5',  '6',  '8',
                '1',  '2',  '5',  '2',  '3',  '1',  '10',  '1',  '1',  '1',  '1',  '1']
g_led_range0 = ['26', '1', '1',  '16', '21', '26',
                '22', '1',  '11', '21', '16', '21', '1',  '29', '1', '1', '1', '1', '1']
g_led_range1 = ['30', '3', '10', '30', '25', '30',
                '24', '15', '20', '25', '30', '30', '10', '30', '5', '15', '5', '5', '10']

G_objects = []
temp_reagent_size = len(g_station)
for i in range(temp_reagent_size):
    g = Grid(
        # reagent_id=g_reag_id[i],
        station=g_station[i],
        layout=g_layout[i],
        pos=g_position[i],
        seg_id=g_led_seg_id[i],
        range0=g_led_range0[i],
        range1=g_led_range1[i],
    )
    G_objects.append(g)

s.bulk_save_objects(G_objects)
try:
    s.commit()
except pymysql.err.IntegrityError as e:
    s.rollback()
except exc.IntegrityError as e:
    s.rollback()
except Exception as e:
    s.rollback()
# ---

# ---supplier table data
super_id = ['1234',   '1201', '2301', '3401',
            '2222', '3333', '6767', '2525', '5555']
super_name = ['貝克曼', '醫全',  '裕利', '大樹', '實用',  '尚上', '伯昂',  '育聖', '亞培']
su1 = Supplier(
    super_id=super_id[0],
    super_name=super_name[0],
    super_address=faker.address(),
    super_connector=faker.name(),
    super_tel=faker.numerify("0#-########"))
s.add(su1)

su2 = Supplier(
    super_id=super_id[1],
    super_name=super_name[1],
    super_address=faker.address(),
    super_connector=faker.name(),
    super_tel=faker.numerify("0#-########"))
s.add(su2)

su3 = Supplier(
    super_id=super_id[2],
    super_name=super_name[2],
    super_address=faker.address(),
    super_connector=faker.name(),
    super_tel=faker.numerify("0#-########"))
s.add(su3)

su4 = Supplier(
    super_id=super_id[3],
    super_name=super_name[3],
    super_address=faker.address(),
    super_connector=faker.name(),
    super_tel=faker.numerify("0#-########"))
s.add(su4)

su5 = Supplier(
    super_id=super_id[4],
    super_name=super_name[4],
    super_address=faker.address(),
    super_connector=faker.name(),
    super_tel=faker.numerify("0#-########"))
s.add(su5)

su6 = Supplier(
    super_id=super_id[5],
    super_name=super_name[5],
    super_address=faker.address(),
    super_connector=faker.name(),
    super_tel=faker.numerify("0#-########"))
s.add(su6)

su7 = Supplier(
    super_id=super_id[6],
    super_name=super_name[6],
    super_address=faker.address(),
    super_connector=faker.name(),
    super_tel=faker.numerify("0#-########"))
s.add(su7)

su8 = Supplier(
    super_id=super_id[7],
    super_name=super_name[7],
    super_address=faker.address(),
    super_connector=faker.name(),
    super_tel=faker.numerify("0#-########"))
s.add(su8)

su9 = Supplier(
    super_id=super_id[8],
    super_name=super_name[8],
    super_address=faker.address(),
    super_connector=faker.name(),
    super_tel=faker.numerify("0#-########"))
s.add(su9)

#s.add_all([su1, su2, su3, su4, su5, su6, su7, su8, su9])

try:
    s.commit()
except pymysql.err.IntegrityError as e:
    s.rollback()
except exc.IntegrityError as e:
    s.rollback()
except Exception as e:
    s.rollback()
# ---

# ---product table data 產品類別
p1 = Product(name='基因檢測試劑')
p2 = Product(name='核酸萃取試劑')
p3 = Product(name='離心機')
p4 = Product(name='C13檢測試劑')
p5 = Product(name='能力試驗')
p6 = Product(name='教育訓練')
p7 = Product(name='抗血清試劑')
p8 = Product(name='血液諮詢')
p9 = Product(name='Microscan細菌鑑定試劑')
p10 = Product(name='台塑生醫EV71-IgM(rapid-tset)')

s.add_all([p1, p2, p3, p4, p5, p6, p7, p8, p9, p10])
try:
    s.commit()
except pymysql.err.IntegrityError as e:
    s.rollback()
except exc.IntegrityError as e:
    s.rollback()
except Exception as e:
    s.rollback()

# 供應商資料
records = s.query(Supplier).all()
#print("total suppliers: ", records)

# 將供應商與產品類別做連結
arrays = [p1, p2, p4, p6, p8]
for array in arrays:
    records[0]._products.append(array)
arrays = [p1, p2, p3, p7]
for array in arrays:
    records[1]._products.append(array)
arrays = [p4, p5, p10]
for array in arrays:
    records[2]._products.append(array)
arrays = [p6, p7, p8, p9]
for array in arrays:
    records[3]._products.append(array)
arrays = [p2, p4, p6, p8]
for array in arrays:
    records[4]._products.append(array)
arrays = [p1, p4, p6, p8]
for array in arrays:
    records[5]._products.append(array)
arrays = [p1, p2, p6, p8]
for array in arrays:
    records[6]._products.append(array)
arrays = [p1, p2, p4]
for array in arrays:
    records[7]._products.append(array)

"""
records = s.query(Product).all()
records[0].supplier_id = [su1, su2]
records[1].supplier_id = [su1, su2]
records[2].supplier_id = [su2]
records[3].supplier_id = [su1, su3]
records[4].supplier_id = [su3]
records[5].supplier_id = [su1, su4]
records[6].supplier_id = [su2, su4]
records[7].supplier_id = [su1, su4]
records[8].supplier_id = [su4]
records[9].supplier_id = [su3]
"""
try:
    s.commit()
except pymysql.err.IntegrityError as e:
    s.rollback()
except exc.IntegrityError as e:
    s.rollback()
except Exception as e:
    s.rollback()

# --------------------------

# ---reagent table data 試劑
reag_id = ['3896124', '3896125', '3802252',
           '3802253', '3896126', '3896090',
           '3802255', '3896127', '3896085',
           '3896089', '2G22.01', '7C18.03',
           '6C32.01', '6C34.01',
           '6C37.02', '8L44.01', '4J27.03', '6C17.03', '2K47.01',
           '2K46.01', '2G22.10', '7C18.13', '6C32.10', '6C34.10',
           '6C37.15', '8L44.10', '4J27.12', '6C17.13', '2K47.20',
           '2K46.10',
           ]
reag_name = ['HBsAg',  'Anti-HBs',   'HBeAg',
             'Anti-HBe',  'Anti-HCV',  'Anti-TPO',
             'Anti-HBc', 'HIV',  'Anti-Rubella IgG',
             'Anti-TG', 'HBsAg Cali   2G22.01',  'Anti-HBs Cali  7C18.03',
             'HBeAg Cali  6C32.01', 'Anti-HBe Cali  6C34.01',
             'Anti-HCV Cali  6C37.02', 'Anti-HBc Cali  8L44.01', 'HIV Cali  4J27.03', 'Anti-Rubella IgG Cali  6C17.03', 'Anti-TPO Cali  2K47.01',
             'Anti-TG Cali  2K46.01 ', 'HBsAg QC  2G22.10', 'Anti-HBs QC  7C18.13', 'HBeAg QC  6C32.10', 'Anti-HBe QC  6C34.10',
             'Anti-HCV QC  6C37.15', 'Anti-HBc QC  8L44.10', 'HIV QC  4J27.12', 'Anti-Rubella IgG QC  6C17.13', 'Anti-TPO QC  2K47.10',
             'Anti-TG QC  2K46.10',
             ]
reag_In_unit = ['組', '組', '組', '組', '組', '組',
                '組', '組', '組', '組', '組', '組',
                '組', '組',
                '組', '組', '組', '組', '組',
                '組', '組', '組', '組', '組',
                '組', '組', '組', '組', '組',
                '組',
                ]
reag_Out_unit = ['組', '組', '組', '組', '組', '組',
                 '組', '組', '組', '組', '組', '組',
                 '組', '組',
                 '組', '組', '組', '組', '組',
                 '組', '組', '組', '組', '組',
                 '組', '組', '組', '組', '組',
                 '組',
                 ]
reag_scale = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
              1, 1, 1, 1, 1,
              1, 1, 1, 1, 1,
              1, 1, 1, 1, 1,
              1,
              ]
reag_period = ['112/10/31', '112/12/31', '112/12/31',
               '112/6/30',  '112/8/31', '112/8/31',
               '112/8/31',  '112/8/31', '112/8/31',
               '112/01/10',  '112/01/10', '112/01/10',
               '112/01/10',  '112/01/10',
               '112/6/30', '112/12/31', '112/12/31', '112/12/31', '112/12/31',
               '112/6/30', '112/12/31', '112/12/31', '112/12/31', '112/12/31',
               '112/12/31', '112/12/31', '112/12/31', '112/6/30', '112/6/30',
               '112/12/31', '111/12/31', '111/12/31', '111/12/31', '111/12/31',
               '111/12/31',
               ]
reag_stock = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
              1, 1, 1, 1, 1,
              1, 1, 1, 1, 1,
              1, 1, 1, 1, 1,
              1,
              ]
reag_temp = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
             1, 1, 1, 1, 2,
             2, 1, 1, 1, 1,
             1, 1, 1, 1, 2,
             2,
             ]  # 0:室溫、1:2~8度C、2:-20度C

reag_catalog = ['試劑組1', '試劑組1', '試劑組1', '試劑組1', '試劑組1',
                '試劑組2', '試劑組2', '試劑組2', '試劑組2', '試劑組2',
                '試劑組3', '試劑組3', '試劑組3', '試劑組3',
                '試劑組1', '試劑組1', '試劑組1', '試劑組1', '試劑組3',
                '試劑組2', '試劑組1', '試劑組1', '試劑組1', '試劑組3',
                '試劑組2', '試劑組1', '試劑組1', '試劑組1', '試劑組3',
                '試劑組2',
                ]

super_id = [9, 9, 9, 9, 9, 9, 9, 9, 9, 9,  9,  9,  9,  9,
            9, 9, 9, 9, 9,
            9, 9, 9, 9, 9,
            9, 9, 9, 9, 9,
            9,
            ]
product_id = [1, 2, 4,   8, 1, 7, 3, 6, 5, 10,  2,  6,  4,  4,
              7, 7, 7, 7, 7,
              7, 7, 7, 7, 7,
              7, 7, 7, 7, 7,
              7,
              ]
grid_id = [1, 2, 3,   4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
           1, 1, 1, 1, 1,
           2, 2, 2, 2, 2,
           3, 3, 3, 3, 3,
           4,
           ]  # 試劑存放於不同格位位置

_objects = []
reag_id_size = len(reag_id)
for x in range(reag_id_size):
    u = Reagent(
        reag_id=reag_id[x],
        reag_name=reag_name[x],
        reag_In_unit=reag_In_unit[x],
        reag_Out_unit=reag_Out_unit[x],
        reag_scale=reag_scale[x],
        reag_period=reag_period[x],
        reag_stock=reag_stock[x],
        reag_temp=reag_temp[x],
        reag_catalog=reag_catalog[x],
        # super_id=super_id[x],
        grid_id=grid_id[x]
    )
    _objects.append(u)

s.bulk_save_objects(_objects)

try:
    s.commit()
except pymysql.err.IntegrityError as e:
    s.rollback()
except exc.IntegrityError as e:
    s.rollback()
except Exception as e:
    s.rollback()

reagent_objects = s.query(Reagent).all()
reagents = [u.__dict__ for u in reagent_objects]
i = 1
for reagent in reagents:
    s.query(Reagent).filter(Reagent.id == i).update(
        {"super_id": super_id[i-1],
         "product_id": product_id[i-1]
         })
    i = i+1

try:
    s.commit()
except pymysql.err.IntegrityError as e:
    s.rollback()
except exc.IntegrityError as e:
    s.rollback()
except Exception as e:
    s.rollback()

s.close()

print("insert 14 grid data is ok...")
print("insert 8 supplier data is ok...")
print("insert 10 product data is ok...")
print("insert 14+16 reagent data is ok...")
