from utils import Logger
log = Logger.getLogger()

def load_txt(fp, data_class=None, as_list=True):
  with open(fp, 'rt') as f:
    lines = f.read().rstrip('\n')
  if as_list:
    lines = lines.split('\n')
  if data_class:
    lines = data_class(**lines)
  return lines

def write_txt(fp, data):
  if not data:
    log.error('no data')
    return
    
  with open(fp, 'w') as f:
    if type(data) == list:
      f.write('\n'.join(data))
    else:
      f.write(data)

def create_a_sketch(data_dict):
  """Creates new Arduino sketch from template"""
  log.debug('Starting process to convert a json to arduino file...')
  
  from os.path import join
  from pathlib import Path
  import shutil
  from glob import iglob
  import os
  
  # Set default options
  opt_board = 'esp32'
  opt_channels = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13]
  opt_wifi_dbm = 19.5

  
  # Craft output file path
  new_filename = Path(data_dict['filename']).stem
  sketch_directory = Path(os.path.dirname(__file__)) / 'networks' / 'sketches' / f"{new_filename}"
  opt_output = sketch_directory 
  opt_output.mkdir(parents=True, exist_ok=True)


  # user input error checks
  if not Path(data_dict['filename']).suffix.lower() == '.json':
    log.error('Only JSON input supported')
    return

  # copy template files
  template_path = join(Path(os.path.dirname(__file__)), 'arduino/templates/skylift_esp32_esp8266/')
  dp_src = template_path
  for fp_src in iglob(join(template_path, '*')):
    fp_dst = join(opt_output, Path(fp_src).name)
    shutil.copyfile(fp_src, fp_dst)
  fp_src = join(opt_output, f'{Path(dp_src).name}.ino')
  fp_dst = join(opt_output, f'{Path(opt_output).name}.ino')
  shutil.move(fp_src, fp_dst)

  # load networks
  networks = data_dict
  wifi_nets = data_dict.get('wifi')

  # read template files
  t = load_txt(fp_dst, as_list=False)

  # replace templated tags
  def insert_template(src, tag, payload):
    tag_start, tag_end = f'// {{{{TEMPLATE:{tag}', f'// TEMPLATE:{tag}}}}}'
    idx_start = t.index(tag_start)
    idx_end = t.index(tag_end) + len(tag_end)
    return src[:idx_start] + str(payload) + src[idx_end:]

  # generate sketch data for template inserts
  templates = {}

  # header
  templates['HEADER'] = f'// Auto-generated from: {networks["filename"]}'

  # number networks
  templates['NN'] = f'#define NN {len(wifi_nets)}'

  # number channels
  templates['N_CHANNELS'] = f'#define N_CHANNELS {len(opt_channels)}'
  channels_str = ", ".join(list(map(str, opt_channels)))
  templates['CHANNELS'] = f'byte channels[N_CHANNELS] = {{{channels_str}}};'

  # esp32|82666
  if opt_board == 'esp32':
    templates['ESP'] = "#define ESP32 1"
  elif opt_board == 'esp8266':
    templates['ESP'] = "#define ESP8266 1"


  # ssids, bssids, channels, dbm_levels
  def bssid_as_hex_list_ino(bssid):
    hex_str = ', '.join([f'0x{x}' for x in bssid.split(':')])
    return '{' + hex_str + '}'
  
  ssids = ['char* ssids[NN] = {']
  bssids = ['byte bssids[NN][6] = {']
  ssid_lengths = ['uint8_t ssid_lengths[NN] = {']
  for wifi_net in wifi_nets:
      ssid_to_use = wifi_net['ssid'] if wifi_net['ssid'] is not None or wifi_net['ssid'] != 'null' else "Hidden Network"
      ssids.append(f'\t"{ssid_to_use}", ')
      ssid_lengths.append(f'\t{len(ssid_to_use)}, ')
      bssids.append(f'\t{bssid_as_hex_list_ino(wifi_net["bssid"])}, ')
  ssids.append('};')
  bssids.append('};')
  ssid_lengths.append('};')
  templates['SSIDS'] = '\n'.join(ssids)
  templates['SSID_LENGTHS'] = '\n'.join(ssid_lengths)
  templates['BSSIDS'] = '\n'.join(bssids)

  import random
  opts_channels = [1, 6, 11]
  use_channels = sorted([random.choice(opts_channels) for x in range(len(wifi_nets))])
  channels = ['byte channels[NN] = {']
  for c in use_channels:
    channels.append(f'\t{c},')
  channels.append('};')

  # wifi power
  templates['WIFI_POWER_DBM'] = f'float WIFI_POWER_DBM = {opt_wifi_dbm};  // ESP8266 only'

  # hidden ssids
  opt_hidden_ssids = False
  if opt_hidden_ssids:
    templates['HIDDEN_SSIDS'] = "#define USE_HIDDEN_SSIDS"
  else:
    templates['HIDDEN_SSIDS'] = "// #define USE_HIDDEN_SSIDS"

  for tag, payload in templates.items():
    t = insert_template(t, tag, payload)

  # write sketch data
  write_txt(fp_dst, t)


  log.info(f'Created "{opt_output.name}" ESP file with {len(wifi_nets)} wifi networks.')