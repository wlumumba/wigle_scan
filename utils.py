from os.path import join
import json
from pathlib import Path
import os
import logging
import sys
import colorlog
import geopy.distance
import requests
import random
import datetime
from enum import Enum
import math
from pprint import pprint
from string import ascii_uppercase
import pandas as pd
import numpy as np
import requests

class Logger:
  
  LOGGER_NAME = 'wigle_scanner'
  LOGFILE_FORMAT = '%(log_color)s%(levelname)-8s%(reset)s %(cyan)s%(filename)s:%(lineno)s %(reset)s%(message)s'

  def __init__(self):
    pass

  @staticmethod
  def create(loglevel=10):
    date_format = '%Y-%m-%d %H:%M:%S'
    if 'colorlog' in sys.modules and os.isatty(2):
      cformat = '%(log_color)s' + Logger.LOGFILE_FORMAT
      f = colorlog.ColoredFormatter(cformat, date_format,
        log_colors = { 'DEBUG'   : 'yellow',       'INFO' : 'white',
        'WARNING' : 'bold_yellow', 'ERROR': 'red',
        'CRITICAL': 'bold_red' })
    else:
      f = logging.Formatter(Logger.LOGFILE_FORMAT, date_format)
    
    logger = logging.getLogger(Logger.LOGGER_NAME)
    logger.setLevel(loglevel)

    # add colored handler
    ch = logging.StreamHandler()
    ch.setFormatter(f)
    logger.addHandler(ch)

    # tests
    # logger.debug('Hello Debug')
    # logger.info('Hello Info')
    # logger.warn('Hello Warn')
    # logger.error('Hello Error')
    # logger.critical('Hello Critical')

    return logger

  @staticmethod
  def getLogger():
    return logging.getLogger(Logger.LOGGER_NAME)

class WigleAPI:
  API_NAME = ""
  API_TOKEN = ""

  def __init__(self):
    self.log = Logger.getLogger()
    self.api_name = WigleAPI.API_NAME
    self.api_token = WigleAPI.API_TOKEN

  def build_url(self, lat, lon, radius_scale, opt_since):
    """Builds Wigle API URL"""
    ## Radius scale of .1 = +50m in all compass directions ##

    radius_inc_lat = 0.00944
    radius_inc_lon = 0.00944
    
    lat_range = (lat - (radius_inc_lat / 2 * radius_scale), lat + (radius_inc_lat / 2 * radius_scale))
    lon_range = (lon - (radius_inc_lon / 2 * radius_scale), lon + (radius_inc_lon / 2 * radius_scale))

    # url builder
    url = 'https://api.wigle.net/api/v2/network/search?'
    url +='onlymine=false&'
    url += 'latrange1=' + str(lat_range[0]) + '&'
    url += 'latrange2=' + str(lat_range[1]) + '&'
    url += 'longrange1=' + str(lon_range[0]) + '&'
    url += 'longrange2=' + str(lon_range[1]) + '&'
    url += 'lastupdt=' + str(int(opt_since)) + '&'
    url += 'resultsPerPage=1000&'
    url += 'freenet=false&'
    url += 'paynet=false'
    return url


  def fetch(self, url, lat, lon):
      """Downloads WiFi data from Wigle API
      Requires API Name/Token
      """
      # https://api.wigle.net/api/v2/network/search?onlymine=false&latrange1=34.09368&latrange2=34.097456&longrange1=-118.380996&longrange2=-118.376404&lastupdt=20160101&freenet=false&paynet=false

      networks = []
      target = (lat, lon)

      # authenticate and get JSON
      # self.log.info('making GET request: {}'.format(url))
      wigle_data = requests.get(url,
        headers={'Authentication':'Basic'},
        auth=(self.api_name, self.api_token))

      # parse wigle results into JSON file
      try:
        wigle_data = wigle_data.json()['results']
      except:
        self.log.error('could not parse data: {}'.format(wigle_data))
        self.log.error(wigle_data)
        self.log.error(f'url: {url}')
        return []

      # Remove networks with 'null' for the SSID value
      for n in wigle_data:
        if n['ssid'] is not None and n['ssid'] != 'null':  # Skip networks with 'null' SSID
          actual = (n['trilat'], n['trilong'])
          rssi_estimated = GeoTools.calc_geo_rssi(actual, target)
          wifi_net = WiFiNet(
            n['ssid'],
            n['netid'],
            n['channel'],
            rssi=rssi_estimated,
            qos=n['qos'],
            lat=n['trilat'],
            lon=n['trilong'],
            lat_target=lat,
            lon_target=lon)
          networks.append(wifi_net)

      # serialize network data
      networks = [n.serialize() for n in networks]
      self.log.debug(f'Wigle API returned {len(networks)} total networks at said coordinates.')
      return networks

class GoogleAPI:
  GAPI_KEY = ""

  def __init__(self, networks):
    self.networks = networks
    self.log = Logger.getLogger()

    if not self.GAPI_KEY:
      self.log.error(f"GoogleAPI: Missing GoogleAPI inside init.")
      raise Exception()

  def check_bssid_status(self, bssid1, bssid2):
    try:
        url = f"https://www.googleapis.com/geolocation/v1/geolocate?key={self.GAPI_KEY}"
        data = {
            "considerIp": "false",
            "wifiAccessPoints": [
                {"macAddress": bssid1},
                {"macAddress": bssid2},
            ],
        }
        headers = {"X-Ios-Bundle-Identifier": "com.example.postman"}
        response = requests.post(url, json=data, headers=headers)
        if response.status_code == 200:
            return True
        elif response.status_code == 404:
            return False
        else:
            raise Exception(f"API status code: {response.status_code}, message: {response.json().get('error').get('message')}")
    except Exception as e:
        self.log.error(f"Failed comparison between {bssid1} and {bssid2}. Please retry. See -> {str(e)}")
        return False
    

  def get_first_valid_network(self):
      valid_network = next(
          (
              network
              for network in self.networks
              for sub_network in self.networks
              if network != sub_network
              and self.check_bssid_status(network["bssid"], sub_network["bssid"])
          ),
          None,
      )

      if not valid_network:
          self.log.warning("No valid networks found at ALL.")
      else:
          self.log.info(f"First valid network chosen for comparisons is... {valid_network['ssid']}")

      return valid_network


  def validate_networks(self, first_valid):
  # Cross-check the valid network aganist all others (excluding self)
    if not first_valid:
      return []

    valid_networks = [first_valid]
    for i, network in enumerate(self.networks):
        if network != first_valid:
            if self.check_bssid_status(network["bssid"], first_valid["bssid"]):
                valid_networks.append(network)
            else:
                self.log.debug(
                    f"index {i} -> {network['ssid']} with bssid: {network['bssid']} was deemed as invalid network."
                )

    # Log summary
    self.log.info(
        f"Out of {len(self.networks)} networks, {len(valid_networks)} were valid networks, therefore {len(self.networks) - len(valid_networks)} are invalidated."
    )
    # self.log.info(f"List of Valid networks: {valid_networks}")

    return valid_networks

class WiFiNet:
  def __init__(self, ssid, bssid, channel, rssi, 
    lat=0.0, lon=0.0, vendor=None, qos=0, lat_target=None, lon_target=None):
    self.ssid = ssid  # (str) 
    self.bssid = bssid  # (str) 00:11:22:33
    self.channel = int(channel)  # wifi channel
    self.rssi = int(rssi) if rssi is not None else rssi  # AP RSSI (not ESP8266 TX power)
    self.qos = qos  # quality of service (only for Wigle)
    self.lat = lat  # actual lat of wifi network
    self.lon = lon  # actual lon of wifi network
    self.vendor = vendor  # MAC vendor
    # target used for Wigle only
    self.lat_target = lat if not lat_target else lat_target
    self.lon_target = lon if not lon_target else lon_target

    p_targ = (self.lat_target, self.lon_target)
    p_actual = (self.lat, self.lon)
    p_y = (self.lat, self.lon_target)  # vary latitude
    p_x = (self.lat_target, self.lon)  # vary longitude
    self.distance_x = GeoTools.get_geo_distance(p_targ, p_x, signed=True)
    self.distance_y = GeoTools.get_geo_distance(p_targ, p_y, signed=True)
    self.distance_xy = GeoTools.get_geo_distance(p_targ, p_actual, signed=True)
    #network['distance_xy'] = math.sqrt( math.pow(x,2) + math.pow(y,2) )  # check if equal?

  def serialize(self):
    return {'ssid':self.ssid,
            'bssid':self.bssid,
            'channel':self.channel,
            'rssi':self.rssi,
            'qos':self.qos,
            'lat':self.lat,
            'lon':self.lon,
            'distance_x':self.distance_x,
            'distance_y':self.distance_y,
            'distance_xy':self.distance_xy,
            }

  def __repr__(self):
    return str(self.serialize())

class GeoTools:
  def get_signed_distance(p1, p2):
    """Returns +/- ? distance (meters) between to lat/lon points"""
    d = geopy.distance.geodesic(p1,p2).m
    return -d if (p2[0] < p1[0] or p2[1] > p1[1]) else d

  def get_geo_distance(p1, p2, signed=False):
    """Returns distance (meters) between to lat/lon points"""
    d = geopy.distance.geodesic(p1, p2).m  # .m for meters 
    return -d if (p2[0] < p1[0] or p2[1] > p1[1]) else d

  def calc_geo_rssi(p1, p2):
    """Estimates RSSI based on geo distance between 2 lat/lon points"""
    m = geopy.distance.geodesic(p1, p2).m
    if m > 1000:
      rssi = -90
    elif m > 500:
      rssi = -80
    elif m > 250:
      rssi = -75
    elif m > 125:
      rssi = -65
    elif m > 50:
      rssi = -55
    else:
      rssi = -50
    return rssi

class NetParser:
  def __init__(self):
    self.log = Logger.getLogger()


  def sort_distance(self, networks, scan_type):
    """Sorts networks by geo distance"""

    # sort based on type of scan
    if scan_type == 'wigle':
      # use distance from target (search origin) location, lowest is strongest
      return sorted(networks, key=lambda x: abs(x['distance_xy']), reverse=False)
    elif scan_type == 'ios' or scan_type == 'wigle_export':
      # use raw RSSI values, highest is strongest
      # networks_sorted = sorted(networks, key=lambda x: x['distance_xy_m'], reverse=False)
      return sorted(networks, key=lambda x: x['rssi'], reverse=True)
    else:
      self.log.error('{} is not a valid type'.format(scan_type))

    
  def filter_rssi(self, networks, rssi_min=None, rssi_max=None):
    """Filters networks by RSSI min/max"""
    if not rssi_min and not rssi_max:
      return networks
    if rssi_min:
      networks = [n for n in networks if float(n['rssi']) > rssi_min]
    if rssi_max:
      networks = [n for n in networks if float(n['rssi']) < rssi_max]
    return networks


  def filter_channel(self, networks, channel_min=None, channel_max=None):
    """Filters networks by channel min/max"""
    if not channel_min and not channel_max:
      return networks
    if channel_min:
      networks = [n for n in networks if float(n['channel']) > channel_min]
    if channel_max:
      networks = [n for n in networks if float(n['channel']) < channel_max]
    return networks


  def filter_quality(self, network_data, qos_min=None, qos_max=None):
    """Filters networks by quality of service (QoS)"""
    if not qos_min and not qos_max:
      return networks
    if qos_min:
      networks = [n for n in networks if float(n['qos']) > qos_min]
    if qos_max:
      networks = [n for n in networks if float(n['qos']) < qos_max]
    return networks

  def publish_json(self, networks, file_name, job):
    # Ensure the directory exists
    results_directory = Path(os.path.dirname(__file__)) / 'networks'
    results_directory.mkdir(exist_ok=True)

    # Construct the base file path with .json suffix
    file_path = results_directory / (file_name + '.json')

    # Check if the file exists and modify the name if it does
    counter = 2
    while file_path.exists():
      new_name = f"{file_name}_{counter}"
      file_path = file_path.with_name(f"{new_name}.json")
      counter += 1

    # Write the networks data to the file
    with open(file_path, 'w') as fp:
        data = {'filename': file_path.name, 'wifi': networks}
        json.dump(data, fp, indent=2, sort_keys=True, ensure_ascii=True)
        self.log.info(f'Successfully saved {file_path.name} to networks folder.')
        
        return data