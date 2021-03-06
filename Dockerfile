FROM ros:kinetic

RUN apt-get -qq update && \
	apt-get install -y --no-install-recommends sudo libavcodec-dev libavutil-dev libgstreamer1.0 libqt5gui5 libqtgui4 libyaml-cpp-dev libpcl-dev python-vtk6 qt5-qmake qtbase5-dev qtbase5-dev-tools qtchooser qtcore4-l10n qttools5-dev qttools5-private-dev tcl-dev tk-dev vtk6 wget && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/*

RUN rosdep update && \
	mkdir -p /catkin_ws/src && \
	bash -c "cd /catkin_ws/src && . /opt/ros/${ROS_DISTRO}/setup.bash && catkin_init_workspace && cd .. && catkin_make"

COPY ./ /catkin_ws/src/neonavigation
RUN /catkin_ws/src/neonavigation/.travis.scripts/test.sh



