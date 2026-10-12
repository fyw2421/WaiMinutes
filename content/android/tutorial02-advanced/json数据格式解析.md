---
title: "json数据格式解析"
weight: 11
description: "json数据格式解析"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

```

```

```

```

```
  
```

```
	  JSONObject rootJsonObject=new JSONObject(jonstring);
```

```
	 
```

```
	  try {
```

```
		  String reason=rootJsonObject.getJSONObject("result").getString("error_code");
```

```
		  
```

```
		  if (reason.equals("200705")||reason.equals("200703")) {
```

```
			
```

```
			return result;
```

```
			  
```

```
			  
```

```
			  
```

```
		}
```

```
	} catch (Exception e) {
```

```
		// TODO: handle exception
```

```
	}
```

```
	  
```

```
	
```

```
	  
```

```
	  Log.v("Jsonpasestep", 2+"");
```

```
	  
```

```
	  JSONArray resultObject=rootJsonObject.getJSONObject("result").getJSONArray("data");
```

```
	  
```

```
	  for(int i=0;i<resultObject.length();i++){
```

```
		  
```

```
		  gasStation station=new gasStation();
```

```
		  
```

```
		  JSONObject object=resultObject.getJSONObject(i);
```

```
		  
```

```
		  
```

```
		  try {
```

```
			  station.setStationName(object.getString("name"));
```

```
		
```

```
			 
```

```
		  
```

```
		  } catch (Exception e) {
```

```
			// TODO: handle exception
```

```
		}
```

```
		  
```

```
		  try {
```

```
			  
```

```
			  station.setPayway(object.getString("fwlsmc"));
```

```
			  
```

```
			} catch (Exception e) {
```

```
				// TODO: handle exception
```

```
			}
```

```
		  try {
```

```
			  station.setLon(object.getString("lon"));		
```

```
			
```

```
			} catch (Exception e) {
```

```
				// TODO: handle exception
```

```
			}
```

```
		  try {
```

```

```

```
			  station.setLat(object.getString("lat"));	
```

```
			
```

```
			} catch (Exception e) {
```

```
				// TODO: handle exception
```

```
			}
```

```
		  try {
```

```
			  station.setDistance(object.getInt("distance"));
```

```
			
```

```
			} catch (Exception e) {
```

```
				// TODO: handle exception
```

```
			}
```

```
		  try {
```

```
			  
```

```
			  station.setBrandName(object.getString("brandname"));		
```

```
			
```

```
		  } catch (Exception e) {
```

```
				// TODO: handle exception
```

```
			}
```

```
		 
```

```
		  try {  station.setAddress(object.getString("address"));
```

```
		 
```

```
			} catch (Exception e) {
```

```
				// TODO: handle exception
```

```
			}
```

```
		  
```

```
		  
```

```
		  
```

```
		 
```

```
		
```

```
		
```

```
		  
```

```
		
```

```
		  
```

```
		
```

```
		 try {
```

```
		
```

```
			 JSONObject priceArray=object.getJSONObject("price");
```

```
			  
```

```
			  String price="";
```

```
			  
```

```
			  Iterator<String> iterator=priceArray.keys();
```

```
			  
```

```
			
```

```
			  while(iterator.hasNext()){
```

```
				 String key=iterator.next();
```

```
				  
```

```
				  
```

```
					 price+=key+":";
```

```
					 
```

```
					 price+=priceArray.getString(key)+"元/升"+"\n";
```

```
					  
```

```
				  
```

```
			  }
```

```
			  
```

```
			  station.setProvinceControlPrice(price);
```

```
		} catch (Exception e) {
```

```
			// TODO: handle exception
```

```
		}
```

```
		  
```

```
 
```

```
		  result.add(station);
```

```
		  
```

```
		  
```

```
	  }
```

```

```

```

```

```

```

```

```

```

```

```

```

```

```

```

```

```
json数据实例
```

```
{
    "resultcode": "200",
    "reason": "Successed!",
    "result": {
        "data": [
            {
                "id": "34299",
                "name": "中油燕宾北邮加油站‎（办卡优惠）",
                "area": "chongwen",
                "areaname": "北京市 崇文区",
                "address": "北京市崇文区天坛路12号，与东市场东街路交叉西南角（天坛北门往西一公里路南）。",
                "brandname": "中石油",
                "type": "加盟店",
                "discount": "打折加油站",
                "exhaust": "京Ⅴ",
                "position": "116.401654,39.886973",
                "lon": "116.40804671453",
                "lat": "39.893324983272",
                "price": [
                    {
                        "type": "E90",
                        "price": "7.31"
                    },
                    {
                        "type": "E93",
                        "price": "6.92"
                    },
                    {
                        "type": "E97",
                        "price": "7.36"
                    },
                    {
                        "type": "E0",
                        "price": "6.84"
                    }
                ],
                "gastprice": [
                    {
                        "name": "92#",
                        "price": "6.77"
                    },
                    {
                        "name": "95#",
                        "price": "7.36"
                    }
                ],
                "fwlsmc": "银联卡,信用卡支付",
                "distance": 2564
            },
            {
                "id": "34316",
                "name": "中油北汽宣武门加油站（办卡优惠）",
                "area": "xuanwu",
                "areaname": "北京市 宣武区",
                "address": "北京市宣武区宣武门西大街28院宣武门地铁西南出口辅路。",
                "brandname": "中石油",
                "type": "加盟店",
                "discount": "打折加油站",
                "exhaust": "京Ⅴ",
                "position": "116.373169,39.899263",
                "lon": "116.37964995964",
                "lat": "39.905320913583",
                "price": [
                    {
                        "type": "E90",
                        "price": "7.31"
                    },
                    {
                        "type": "E93",
                        "price": "6.92"
                    },
                    {
                        "type": "E97",
                        "price": "7.36"
                    },
                    {
                        "type": "E0",
                        "price": "6.84"
                    }
                ],
                "gastprice": [
                    {
                        "name": "92#",
                        "price": "6.92"
                    },
                    {
                        "name": "95#",
                        "price": "7.36"
                    }
                ],
                "fwlsmc": "银联卡,信用卡支付,加油卡,便利店,洗车服务",
                "distance": 2332
            },
            {
                "id": "34374",
                "name": "中石化宣武门加油站",
                "area": "xuanwu",
                "areaname": "北京市 宣武区",
                "address": "北京市宣武区宣武门东大街，（北京市急救中心对面，北京市教委斜对面。崇光百货的北面）。",
                "brandname": "中石化",
                "type": "直营店",
                "discount": "打折加油站",
                "exhaust": "京Ⅴ",
                "position": "116.3724501688,39.8936984142",
                "lon": "116.38527024009",
                "lat": "39.905558817477",
                "price": [
                    {
                        "type": "E90",
                        "price": "7.31"
                    },
                    {
                        "type": "E93",
                        "price": "6.92"
                    },
                    {
                        "type": "E97",
                        "price": "7.36"
                    },
                    {
                        "type": "E0",
                        "price": "6.84"
                    }
                ],
                "gastprice": [
                    {
                        "name": "92#",
                        "price": "6.92"
                    },
                    {
                        "name": "95#",
                        "price": "7.36"
                    }
                ],
                "fwlsmc": "银联卡,信用卡支付,加油卡,便利店,发卡充值网点,卫生间,银联卡充值,移动二维码业务,加油卡充值业务,会员卡服务,公交卡充值,出租车数据采集",
                "distance": 1920
            },
            {
                "id": "46231",
                "name": "中石化崇文门加油站",
                "area": "chongwen",
                "areaname": "北京市 崇文区",
                "address": "北京市崇文区崇文门东大街甲22号崇文门地铁站以东400米处,崇文门饭店东面。",
                "brandname": "中石化",
                "type": "直营店",
                "discount": "打折加油站",
                "exhaust": "京Ⅴ",
                "position": "116.421336,39.900329",
                "lon": "116.42778321554",
                "lat": "39.906481951762",
                "price": [
                    {
                        "type": "E90",
                        "price": "7.31"
                    },
                    {
                        "type": "E93",
                        "price": "6.92"
                    },
                    {
                        "type": "E97",
                        "price": "7.36"
                    },
                    {
                        "type": "E0",
                        "price": "6.84"
                    }
                ],
                "gastprice": [
                    {
                        "name": "92#",
                        "price": "6.92"
                    },
                    {
                        "name": "95#",
                        "price": "7.36"
                    }
                ],
                "fwlsmc": "加油卡,便利店,92#自助加油,95#自助加油,发卡充值网点,银联卡充值,移动二维码业务,加油卡充值业务,出租车数据采集",
                "distance": 2360
            },
            {
                "id": "34280",
                "name": "中石化北京站加油站",
                "area": "dongcheng",
                "areaname": "北京市 东城区",
                "address": "北京市东城区西镇江胡同北京站西街路西北。",
                "brandname": "中石化",
                "type": "直营店",
                "discount": "打折加油站",
                "exhaust": "京Ⅴ",
                "position": "116.422892,39.904058",
                "lon": "116.42934447783",
                "lat": "39.910183641108",
                "price": [
                    {
                        "type": "E90",
                        "price": "7.31"
                    },
                    {
                        "type": "E93",
                        "price": "6.92"
                    },
                    {
                        "type": "E97",
                        "price": "7.36"
                    },
                    {
                        "type": "E0",
                        "price": "6.84"
                    }
                ],
                "gastprice": [
                    {
                        "name": "0#",
                        "price": "6.84"
                    },
                    {
                        "name": "92#",
                        "price": "6.92"
                    },
                    {
                        "name": "95#",
                        "price": "7.36"
                    }
                ],
                "fwlsmc": "加油卡,便利店,92#自助加油,95#自助加油,发卡充值网点,移动二维码业务,加油卡充值业务",
                "distance": 2332
            },
            {
                "id": "34359",
                "name": "加油站‎（东四南大街）",
                "area": "dongcheng",
                "areaname": "北京市 东城区",
                "address": "北京东城区东四南大街。",
                "brandname": "不详",
                "type": "其他",
                "discount": "非打折加油站",
                "exhaust": "京Ⅴ",
                "position": "116.415918,39.910041",
                "lon": "116.42232800867",
                "lat": "39.916272549494",
                "price": [
                    {
                        "type": "E90",
                        "price": "7.31"
                    },
                    {
                        "type": "E93",
                        "price": "6.92"
                    },
                    {
                        "type": "E97",
                        "price": "7.36"
                    },
                    {
                        "type": "E0",
                        "price": "6.84"
                    }
                ],
                "gastprice": [
                    {
                        "name": "0#",
                        "price": "6.84"
                    },
                    {
                        "name": "92#",
                        "price": "6.92"
                    },
                    {
                        "name": "95#",
                        "price": "7.36"
                    }
                ],
                "fwlsmc": "",
                "distance": 1640
            },
            {
                "id": "34297",
                "name": "中石化金融街加油站",
                "area": "xicheng",
                "areaname": "北京市 西城区",
                "address": "北京市西城区太平桥大街路西侧与二龙路西街交口处。",
                "brandname": "中石化",
                "type": "直营店",
                "discount": "打折加油站",
                "exhaust": "京Ⅴ",
                "position": "116.362794,39.912337",
                "lon": "116.36932672496",
                "lat": "39.918204483855",
                "price": [
                    {
                        "type": "E90",
                        "price": "7.31"
                    },
                    {
                        "type": "E93",
                        "price": "6.92"
                    },
                    {
                        "type": "E97",
                        "price": "7.36"
                    },
                    {
                        "type": "E0",
                        "price": "6.84"
                    }
                ],
                "gastprice": [
                    {
                        "name": "92#",
                        "price": "6.92"
                    },
                    {
                        "name": "95#",
                        "price": "7.36"
                    }
                ],
                "fwlsmc": "银联卡,信用卡支付,加油卡,便利店,发卡充值网点,卫生间,银联卡充值,移动二维码业务,加油卡充值业务,会员卡服务,公交卡充值,出租车数据采集",
                "distance": 2895
            },
            {
                "id": "75156",
                "name": "中石油朝阳加油站",
                "area": "",
                "areaname": "北京市 东城区",
                "address": "",
                "brandname": "中石油",
                "type": "其他",
                "discount": "打折加油站",
                "exhaust": "京Ⅴ",
                "position": "116.42194748,39.91724124",
                "lon": "116.4283836199",
                "lat": "39.923379083879",
                "price": [
                    {
                        "type": "E90",
                        "price": "7.31"
                    },
                    {
                        "type": "E93",
                        "price": "6.92"
                    },
                    {
                        "type": "E97",
                        "price": "7.36"
                    },
                    {
                        "type": "E0",
                        "price": "6.84"
                    }
                ],
                "gastprice": [
                    {
                        "name": "0#",
                        "price": "6.84"
                    },
                    {
                        "name": "92#",
                        "price": "6.58"
                    }
                ],
                "fwlsmc": "信用卡支付",
                "distance": 2306
            },
            {
                "id": "34360",
                "name": "中国石化 (中国新闻出版总署东南)...",
                "area": "dongcheng",
                "areaname": "北京市 东城区",
                "address": "北京市东四南大街106号中华人民共和国新闻出版总署东南。",
                "brandname": "中石化",
                "type": "直营店",
                "discount": "打折加油站",
                "exhaust": "京Ⅴ",
                "position": "116.417775,39.92043",
                "lon": "116.42418687653",
                "lat": "39.926632910538",
                "price": [
                    {
                        "type": "E90",
                        "price": "7.31"
                    },
                    {
                        "type": "E93",
                        "price": "6.92"
                    },
                    {
                        "type": "E97",
                        "price": "7.36"
                    },
                    {
                        "type": "E0",
                        "price": "6.84"
                    }
                ],
                "gastprice": [
                    {
                        "name": "0#",
                        "price": "6.84"
                    },
                    {
                        "name": "92#",
                        "price": "6.92"
                    }
                ],
                "fwlsmc": "加油卡",
                "distance": 2151
            },
            {
                "id": "34291",
                "name": "中石化景山加油站",
                "area": "dongcheng",
                "areaname": "北京市 东城区",
                "address": "北京市东城景山后街7号,区景山后街东段路北,与地安门内大街交叉口东。",
                "brandname": "中石化",
                "type": "直营店",
                "discount": "打折加油站",
                "exhaust": "京Ⅴ",
                "position": "116.397631,39.928785",
                "lon": "116.40399325389",
                "lat": "39.935125033327",
                "price": [
                    {
                        "type": "E90",
                        "price": "7.31"
                    },
                    {
                        "type": "E93",
                        "price": "6.92"
                    },
                    {
                        "type": "E97",
                        "price": "7.36"
                    },
                    {
                        "type": "E0",
                        "price": "6.84"
                    }
                ],
                "gastprice": [
                    {
                        "name": "92#",
                        "price": "6.92"
                    }
                ],
                "fwlsmc": "加油卡,便利店,发卡充值网点,移动二维码业务,加油卡充值业务,出租车数据采集",
                "distance": 2126
            },
            {
                "id": "15955",
                "name": "中石化平安大街加油站‎ ‎",
                "area": "xicheng",
                "areaname": "北京市 西城区",
                "address": "北京市西城区西城区地安门西大街，北海北门向西，教场胡同里。",
                "brandname": "中石化",
                "type": "直营店",
                "discount": "打折加油站",
                "exhaust": "京Ⅴ",
                "position": "116.386505,39.932882",
                "lon": "116.39289518126",
                "lat": "39.93914114521",
                "price": [
                    {
                        "type": "E90",
                        "price": "7.31"
                    },
                    {
                        "type": "E93",
                        "price": "6.92"
                    },
                    {
                        "type": "E97",
                        "price": "7.36"
                    },
                    {
                        "type": "E0",
                        "price": "6.84"
                    }
                ],
                "gastprice": [
                    {
                        "name": "0#",
                        "price": "6.84"
                    },
                    {
                        "name": "92#",
                        "price": "6.92"
                    },
                    {
                        "name": "95#",
                        "price": "7.36"
                    }
                ],
                "fwlsmc": "银联卡,加油卡,便利店,柴油自助加油,发卡充值网点,银联卡充值,移动二维码业务,加油卡充值业务,出租车数据采集",
                "distance": 2715
            }
        ],
        "pageinfo": {
            "pnums": 20,
            "current": "1",
            "allpage": 1
        }
    },
    "error_code": 0
}
```
