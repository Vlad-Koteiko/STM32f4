FROM python:3.11

RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y gcc-arm-none-eabi cmake 

RUN pip install conan && conan profile detect

ENTRYPOINT ["bash"]
