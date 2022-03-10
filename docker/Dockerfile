FROM ubuntu:focal

RUN apt-get update \
 && apt-get install -y \
      sudo wget lsb-release gnupg curl \
 && apt-get clean

ARG user_id
ENV USERNAME developer
RUN useradd -U --uid ${user_id} -ms /bin/bash $USERNAME \
 && echo "$USERNAME:$USERNAME" | chpasswd \
 && adduser $USERNAME sudo \
 && echo "$USERNAME ALL=NOPASSWD: ALL" >> /etc/sudoers.d/$USERNAME

# Commands below run as the developer user
USER $USERNAME

WORKDIR /home/$USERNAME

COPY scripts/enable_ign_stable.sh scripts/enable_ign_stable.sh
RUN sudo scripts/enable_ign_stable.sh

COPY scripts/install_common_deps.sh scripts/install_common_deps.sh
RUN scripts/install_common_deps.sh

# This is not strictly necessary, but makes things faster down the line.
COPY scripts/install_ign_deps.sh scripts/install_ign_deps.sh
RUN scripts/install_ign_deps.sh

COPY scripts/build_ign.sh scripts/build_ign.sh
COPY scripts/build.sh scripts/build.sh
RUN scripts/build.sh

ADD ign-test /home/$USERNAME/ign-test
RUN sudo chown -R $USERNAME:$USERNAME /home/$USERNAME/ign-test
RUN cd ign-test; mkdir build; 
WORKDIR /home/$USERNAME/ign-test/build
RUN cmake ../ -DBUILD_TESTING=false; sudo make install

COPY run.bash ./
ENTRYPOINT ["./run.bash"]
