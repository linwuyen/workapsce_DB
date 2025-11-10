import googlemaps

# 替換 YOUR_API_KEY 為你的 Google Maps API 密鑰
gmaps = googlemaps.Client(key='YOUR_API_KEY')

# 地址查詢
address = '101號, Section 5, Zhongxiao East Road, Xinyi District, Taipei City, Taiwan 110'

# 進行地理編碼
geocode_result = gmaps.geocode(address)

# 輸出結果
print(geocode_result)
