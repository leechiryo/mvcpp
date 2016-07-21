var vm = new Vue({
  el: '#app',
  data: {
    message: 'Hello Vue.js!',
    greeting: true
  },
  methods: {
    addTodo: function () {
      var text = this.message;
    }
  }
});
