import time
import logging
import os
import json


def init_log():
    log_config = dict(filename=os.path.basename(__file__).split('.')[0] + '.log',
                      filemode='w',
                      level=logging.DEBUG,
                      format='%(asctime)-15s %(levelname)s %(filename)s %(lineno)d %(process)d %(message)s',
                      datefmt='%a %d %b %Y %H:%M:%S')
    logging.basicConfig(**log_config)


def test_func1():
    print('this is test_func1')
    for i in range(5):
        print('Hello World! (count #{}). This is from'.format(i), os.path.basename(__file__))
        time.sleep(0.1)


def test_func2(json_in):
    print('this is test_func2')
    print('input = ', json_in)
    data = json.loads(json_in)
    data['no'] = 2  # modify a key-value pair
    data['a'] = 'b'  # add a new key-value pair
    json_out = json.dumps(data)
    return json_out


if __name__ == '__main__':
    init_log()
    logging.debug('starting...')
    test_func1()
    logging.debug('end')
