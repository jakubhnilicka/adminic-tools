module.exports = function(grunt) {

  // Project configuration.
  grunt.initConfig({

    pkg: grunt.file.readJSON('package.json'),

    watch: {
      css: {
        files: ['**/*.scss'],
        tasks: ['sass'],
        options: {
          livereload: true,
          spawn: false
        },
      },
      twig: {
        files: ['**/*.twig'],
        options: {
          livereload: true,
        },
      },
    },

    sass: {
        options: {
            sourceMap: true,
            outputStyle: 'compressed',
            sourceComments: false
        },
        dist: {
            files: {
                'css/<%= pkg.name %>.css': 'scss/<%= pkg.name %>.scss'
            }
        }
    },

  });

  // Load plugins
  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-sass');

  grunt.registerTask('default', ['sass']);

};
/*<script src="//localhost:35729/livereload.js"></script>*/
