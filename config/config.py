import os
import json

CONFIG_FILE = os.path.join("config.json")

#  TODO what is the type of None ?
BASIC_TYPES = [str, bool, int, float]
def isbasictype(v):
    return any(isinstance(v, typ) for typ in BASIC_TYPES)


class ConfigError(ValueError):
    pass


def init_file(overwrite=False):
    if not overwrite:
        if os.path.isfile(CONFIG_FILE):
            raise ConfigError(f"Couldn't intizialise the configuration file '{CONFIG_FILE}' already exists. Use the 'overwrite' flag to ignore this warning.")

    with open(CONFIG_FILE, "w+") as f:
        f.write("{}")

def set_key(path, value, create_nodes=False, overwrite=True, overwrite_path=False):

    assert isinstance(path, list)
    assert all(isinstance(v, str) for v in path)
    assert isbasictype(value)
    assert isinstance(overwrite, bool)

    if overwrite_path:
        overwrite = True


    with open(CONFIG_FILE, "r") as f:
        root = json.load(f)
        assert isinstance(root, dict)

        # Getting to the key
        node = root
        for i, s in enumerate(path[:-1]):
            if s not in node:
                if create_nodes:
                    node[s] = dict()
                else:
                    raise ConfigError(f"The path '{':'.join(path)}' doesn't exists within the configuration file '{CONFIG_FILE}': key '{s}' at position {i} wasn't found.")
            node = node[s]

        # Modifing the key.
        final_key = path[-1]

        if not overwrite and final_key in node:
            if isbasictype(node[final_key]):
                raise ConfigError(f"The path '{':'.join(path)}' erase an existing path within the configuration file '{CONFIG_FILE}' : key '{final_key}' already exists. use the 'overwrite' tag to overwrite.")
            else:
                raise ConfigError(f"The path '{':'.join(path)}' erase an existing path within the configuration file '{CONFIG_FILE}' : key '{final_key}' already exists. use the 'overwrite_path' tag to overwrite.")
        if not overwrite_path and final_key in node:
            if not isbasictype(node[final_key]):
                raise ConfigError(f"The path '{':'.join(path)}' erase an existing path within the configuration file '{CONFIG_FILE}' : key '{final_key}' already exists. use the 'overwrite_path' tag to overwrite.")


        node[final_key] = value

    with open(CONFIG_FILE, "w+") as f:
        json.dump(root, f)


def get_key(path):

    assert isinstance(path, list)
    assert all(isinstance(v, str) for v in path)

    with open(CONFIG_FILE, "r") as f:
        root = json.load(f)
        assert isinstance(root, dict)

        # Getting to the key
        node = root
        for i, s in enumerate(path):
            if s not in node:
                raise ConfigError(f"The path '{':'.join(path)}' doesn't exists within the configuration file '{CONFIG_FILE}': key '{s}' at position {i} wasn't found.")
            node = node[s]

        # returing the value associated with the key
        value = node
        if not isbasictype(value):
            raise ConfigError(f"The path '{':'.join(path)}' is not a basic type within the configuration file '{CONFIG_FILE}'.")

    return value

def main():
    set_key(["test", "test", "test"], 1, create_nodes=True)
    set_key(["test", "test", "another_test"], 2, create_nodes=True)
    print(get_key(["test", "test", "test"]))

if __name__ == '__main__':
    main()