window.loadGameOverScript = function() {
    window.addEventListener('keydown', (event) => {
    if (event.code === 'Enter') {
        window.location.reload();
    }
});
};