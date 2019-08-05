pipeline {
  agent {
    docker {
      image 'jenkins_clang8_image'
    }

  }
  stages {
    stage('init') {
      steps {
        sh '''export PATH=/clang_8.0.0/bin:$PATH
export CC=/clang_8.0.0/bin/clang
export CXX=/clang_8.0.0/bin/clang++
export LD_LIBRARY_PATH=/clang_8.0.0/lib:$LD_LIBRARY_PATH rm -rf *'''
      }
    }
    stage('configure') {
      steps {
        sh '''cd test
mkdir build
cd build
cmake ..'''
      }
    }
    stage('build') {
      steps {
        sh 'cd  build make'
      }
    }
    stage('test') {
      steps {
        sh 'cd build make run_tests'
      }
    }
  }
}